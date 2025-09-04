// Copyright Sertim all rights reserved


#include "Actor/CustodianProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Custodian/Custodian.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Perception/AISenseConfig_Damage.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


ACustodianProjectile::ACustodianProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void ACustodianProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileNiagaraSystem)
	{
		SpawnedNiagaraSystem = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ProjectileNiagaraSystem,
			this->GetRootComponent(),
			FName(""),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			false,
			true,
			ENCPoolMethod::None,
			true);
	}
	if (ProjectileParticleEffect)
	{
		SpawnedParticleEffect = UGameplayStatics::SpawnEmitterAttached(
			ProjectileParticleEffect,
			this->GetRootComponent(),
			FName(""),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			false
			);
	}
	if (LoopingSound)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	}
	SetLifeSpan(LifeSpan);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACustodianProjectile::OnSphereOverlap);
}

void ACustodianProjectile::Destroyed()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if (!bHit || !HasAuthority()) OnHit();
	Super::Destroyed();
}

void ACustodianProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect, GetActorLocation());
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if (SpawnedNiagaraSystem)
	{
		SpawnedNiagaraSystem->Deactivate();
		SpawnedNiagaraSystem->DestroyComponent();
	}
	if (SpawnedParticleEffect)
	{
		SpawnedParticleEffect->Deactivate();
		SpawnedParticleEffect->DestroyComponent();
	}
	bHit = true;
}

void ACustodianProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromResult, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (!bHit) OnHit();

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

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
			UCustodianAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);

			// Report Damage Event for AI 
			// P.S. do not put 0 in damage amount!
			AActor* SourceActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
			AActor* TargetActor = TargetASC->GetAvatarActor();
			UAISense_Damage::ReportDamageEvent(GetWorld(), TargetActor, SourceActor, 1.f, SourceActor->GetActorLocation(), TargetActor->GetActorLocation());
		}
		Destroy();
	}
	else bHit = true;
}

bool ACustodianProjectile::IsValidOverlap(AActor* OtherActor)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return false;
	return true;
}

