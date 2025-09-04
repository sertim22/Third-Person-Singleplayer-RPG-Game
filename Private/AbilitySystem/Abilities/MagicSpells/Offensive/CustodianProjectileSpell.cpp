// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/MagicSpells/Offensive/CustodianProjectileSpell.h"
#include "Actor/CustodianProjectile.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Interaction/PlayerInterface.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


void UCustodianProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanActivateAbility())
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else return;
}

void UCustodianProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	ACustodianProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACustodianProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->SetOwner(GetAvatarActorFromActorInfo());
	Projectile->SetInstigator(Cast<APawn>(GetAvatarActorFromActorInfo()));
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	Projectile->FinishSpawning(SpawnTransform);
	Projectile->Sphere->IgnoreActorWhenMoving(GetAvatarActorFromActorInfo(), true);
}

TArray<ACustodianProjectile*> UCustodianProjectileSpell::SpawnBarrageProjectiles(const FGameplayTag& SocketTag, AActor* HomingTarget)
{
	TArray<ACustodianProjectile*> SpawnedFireBolts;
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return SpawnedFireBolts;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotation = (HomingTarget->GetActorLocation() - SocketLocation).Rotation();

	const FVector Forward = Rotation.Vector();
	const int32 EffectiveNumProjectiles = FMath::Min(MaxNumProjectiles, 4/*GetAbilityLevel()*/);
	TArray<FRotator> Rotations = UCustodianAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		ACustodianProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACustodianProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		SpawnedFireBolts.Add(Projectile);

		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
		Projectile->ProjectileMovement->ProjectileGravityScale = 0.f;
		Projectile->ProjectileMovement->InitialSpeed = ProjectileInitialSpeed;
		Projectile->ProjectileMovement->MaxSpeed = ProjectileMaxSpeed;
		Projectile->SetOwner(GetAvatarActorFromActorInfo());
		Projectile->SetInstigator(Cast<APawn>(GetAvatarActorFromActorInfo()));
		Projectile->ProjectileMovement->Deactivate();
		Projectile->AttachToComponent(Cast<ACharacter>(GetAvatarActorFromActorInfo())->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachmentSocketName);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		Projectile->FinishSpawning(SpawnTransform);
		Projectile->Sphere->IgnoreActorWhenMoving(GetAvatarActorFromActorInfo(), true);
	}

	return SpawnedFireBolts;
}

void UCustodianProjectileSpell::LaunchProjectilesOverTime(TArray<ACustodianProjectile*> InSpawnedProjectiles, AActor* InHomingTarget, float InTime)
{
	if (InSpawnedProjectiles.Num() == 0)
	{
		return;
	}

	// Launch first projectile to start launching the rest if there are more than 1 projectile
	LaunchSingleProjectile(InSpawnedProjectiles, InHomingTarget, InTime, 0);
}

void UCustodianProjectileSpell::LaunchSingleProjectile(TArray<ACustodianProjectile*> InSpawnedProjectiles, AActor* InHomingTarget, float InTime, int32 CurrentIndex)
{
	if (CurrentIndex >= InSpawnedProjectiles.Num())
	{
		return; // check if all projectiles have been launched to stop the cicle
	}

	// Get Projectile at the current array index
	ACustodianProjectile* Projectile = InSpawnedProjectiles[CurrentIndex];

	if (InHomingTarget && InHomingTarget->Implements<UCombatInterface>())
	{
		Projectile->ProjectileMovement->HomingTargetComponent = InHomingTarget->GetRootComponent();
	}
	else
	{
		Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
		Projectile->HomingTargetSceneComponent->SetWorldLocation(InHomingTarget->GetActorLocation());
		Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
	}
	Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
	Projectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Projectile->ProjectileMovement->Activate();
	Projectile->SetLifeSpan(BarrageProjectileLifeSpan);

	// Launch the next projectile after the expiration of timer
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		FTimerDelegate::CreateUObject(
			this,
			&UCustodianProjectileSpell::LaunchSingleProjectile, // Call the same function via creating simple TimerDelegate
			InSpawnedProjectiles,
			InHomingTarget,
			InTime,
			CurrentIndex + 1
		),
		InTime,
		false
	);
}

FVector UCustodianProjectileSpell::CalculateProjectileDirection(ACustodianProjectile* InProjectile)
{
	UCameraComponent* Camera = GetAvatarActorFromActorInfo()->GetComponentByClass<UCameraComponent>();
	const FVector Start = Camera->K2_GetComponentLocation();
	const FVector End = Camera->GetForwardVector() * 10000.f + Start;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(InProjectile);
	FHitResult HitResult;

	bool bSelectVector = UKismetSystemLibrary::LineTraceSingle(
		GetAvatarActorFromActorInfo(),
		Start,
		End,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	FVector SelectedVector = UKismetMathLibrary::SelectVector(HitResult.Location, HitResult.TraceEnd, bSelectVector);
	const FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(InProjectile->GetActorLocation(), SelectedVector);
	return Direction;
}

bool UCustodianProjectileSpell::CanActivateAbility()
{
	bool bIsPlayer = GetAvatarActorFromActorInfo()->Implements<UPlayerInterface>();
	if (bIsPlayer)
	{
		if (UCustodianAbilitySystemLibrary::IsInMagicStance(Cast<ACharacter>(GetAvatarActorFromActorInfo()))
			&& UCustodianAbilitySystemLibrary::IsAiming(Cast<ACharacter>(GetAvatarActorFromActorInfo())))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (UCustodianAbilitySystemLibrary::IsInMagicStance(Cast<ACharacter>(GetAvatarActorFromActorInfo())))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
