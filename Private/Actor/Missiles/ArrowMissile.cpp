// Copyright Sertim all rights reserved


#include "Actor/Missiles/ArrowMissile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Custodian/Custodian.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Perception/AISense_Damage.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/InventoryInterface.h"
#include "Inventory/CustodianInventoryComponent.h"


AArrowMissile::AArrowMissile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECC_Projectile);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	MissileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	MissileMovement->bRotationFollowsVelocity = true;
	MissileMovement->ProjectileGravityScale = 0.f;
	MissileMovement->Deactivate();
}

void AArrowMissile::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AArrowMissile::OnBoxOverlap);
}

void AArrowMissile::Destroyed()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if (!bHit && !HasAuthority()) OnHit();
	LifeSpanExpired();
	Super::Destroyed();
}

void AArrowMissile::OnHit()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if (NiagaraEffect)
	{
		NiagaraEffect->Deactivate();
	}
	bHit = true;
}

void AArrowMissile::CalculateMissileSpeed(const FVector& Direction, UProjectileMovementComponent* InProjectileMovement, FVector& InVelocity, float Strength)
{
	InVelocity = Direction * FMath::Lerp(MinSpeed, MaxSpeed, Strength);
	InProjectileMovement->Velocity = InVelocity;

	InProjectileMovement->ProjectileGravityScale = FMath::Lerp(MaxGravity, MinGravity, Strength);
}

void AArrowMissile::EnableArrowEffectsAndSounds()
{
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetLifeSpan(LifeSpan);
	UGameplayStatics::PlaySoundAtLocation(this, WooshSound, this->GetActorLocation());
	NiagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
		TrailEffect,
		this->GetRootComponent(),
		FName(""),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		false,
		true,
		ENCPoolMethod::None,
		true);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AArrowMissile::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromResult, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (!bHit) OnHit();
	
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	if(MissileMovement)
	{
		MissileMovement->StopMovementImmediately();
		MissileMovement->ProjectileGravityScale = 0.0f;
		MissileMovement->DestroyComponent();
	}
	AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);

	// Get Owner of the Missile
	AActor* SourceActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if (bKnockback)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;

				const FVector KnockbackDirection = Rotation.Vector();
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
				DamageEffectParams.KnockbackForce = KnockbackForce;
			}
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;

			// Make sure that the damage will be caused by the RangedWeapon currently equipped on the character
			IInventoryInterface::Execute_SetRangedWeaponTag(SourceActor, IInventoryInterface::Execute_GetCustodianInventoryComponent(SourceActor)->RangedWeaponTag);
			ICombatInterface::Execute_SetShouldApplyRangedDamage(SourceActor, true);
			// Apply Damage 
			UCustodianAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);

			// Report Damage Event for AI 
			// P.S. do not put 0 in damage amount! It will cause crashes
			AActor* TargetActor = TargetASC->GetAvatarActor();
			UAISense_Damage::ReportDamageEvent(GetWorld(), TargetActor, SourceActor, 1.f, SourceActor->GetActorLocation(), TargetActor->GetActorLocation());

			UParticleSystem* BloodEffect = ICombatInterface::Execute_GetBloodEffect(TargetActor);

			if (BloodEffect && FleshHitSound)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, BloodEffect, GetActorLocation());
				UGameplayStatics::PlaySoundAtLocation(this, FleshHitSound, GetActorLocation(), FRotator::ZeroRotator);
			}
			// Set "bShouldApplyRangedDamage" back to false, in order to avoid mistakes when calculating damage
			ICombatInterface::Execute_SetShouldApplyRangedDamage(SourceActor, false);
			
		}
		else
		{
			if (ImpactEffect && ImpactSound)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect, GetActorLocation());
				UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
			}
		}
		// Set "RangedDamageMultiplier" to 0 , in order to avoid mistakes when calculating damage
		ICombatInterface::Execute_SetRangedDamageMultiplier(SourceActor, 0.f);
	}
	else bHit = true;
	ICombatInterface::Execute_SetRangedDamageMultiplier(SourceActor, 0.f);
}

bool AArrowMissile::IsValidOverlap(AActor* OtherActor)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return false;
	return true;
}

