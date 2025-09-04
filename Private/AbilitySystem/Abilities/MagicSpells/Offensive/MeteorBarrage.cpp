// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/MagicSpells/Offensive/MeteorBarrage.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/CustodianProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UMeteorBarrage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanActivateAbility())
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else return;
}

void UMeteorBarrage::PlayerSpawnMeteors()
{

}

void UMeteorBarrage::AISpawnMeteors(AActor* InCombatTarget)
{
	FVector AvatarLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	FVector TargetLocation = InCombatTarget->GetActorLocation();

	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	bool bSelectVector = UKismetSystemLibrary::LineTraceSingle(
		GetAvatarActorFromActorInfo(),
		AvatarLocation,
		AvatarLocation + FVector(0.f, 0.f, MeteorCloudZOffset),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true);

	FVector SelectedVector = bSelectVector ? HitResult.Location : HitResult.TraceEnd;

	AvatarLocation.Z = 0.f;
	TargetLocation.Z = 0.f;

	FVector MiddlePoint = (AvatarLocation + TargetLocation) * 0.5f;

	FVector InitialSpawnLocation = FVector(MiddlePoint.X, MiddlePoint.Y, SelectedVector.Z);
	//DrawDebugSphere(GetWorld(), InitialSpawnLocation, 100.f, 12, FColor::Red, true);
	SetMeteorSpawnLocation(InitialSpawnLocation);
	if (MeteorCloud)
	{
		SpawnedCloudEmitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorCloud, MeteorCloudSpawnLocation);
	}
	
	SpawnMeteorsOverTime(MeteorClass, 0, InCombatTarget);
}

void UMeteorBarrage::SpawnMeteorsOverTime(TSubclassOf<ACustodianProjectile> InMeteorClass, int32 CurrentIndex, AActor* InCombatTarget)
{
	int32 EffectiveNumMeteors = FMath::Min(5/*GetAbilityLevel() + 2*/, MaxNumMeteors);
	if (CurrentIndex >= EffectiveNumMeteors)
	{
		bMeteorsEnded = true;
		if (SpawnedCloudEmitter)
		{
			SpawnedCloudEmitter->Deactivate();
			SpawnedCloudEmitter->DestroyComponent();
		}
		OnMeteorsEndedDelegate.Broadcast(bMeteorsEnded);
		return; // check if all projectiles have been launched to stop the cicle and broadcast the delegate
	}

	FVector InitialSpawnLocation = MeteorCloudSpawnLocation;
	FVector RandomMeteorDestination = InCombatTarget->GetActorLocation() + FVector(FMath::RandRange(MinMeteorXOffset, MaxMeteorXOffset), FMath::RandRange(MinMeteorYOffset, MaxMeteorYOffset), InCombatTarget->GetActorLocation().Z);
	FRotator InitialSpawnRotation = (RandomMeteorDestination - InitialSpawnLocation).Rotation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(InitialSpawnLocation);
	SpawnTransform.SetRotation(InitialSpawnRotation.Quaternion());

	// Get Projectile at the current array index
	ACustodianProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACustodianProjectile>(
		InMeteorClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	Projectile->SetInstigator(Cast<APawn>(GetAvatarActorFromActorInfo()));
	Projectile->SetOwner(GetAvatarActorFromActorInfo());
	Projectile->SetLifeSpan(MeteorLifeSpan);
	Projectile->Sphere->IgnoreActorWhenMoving(GetAvatarActorFromActorInfo(), true);

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

	Projectile->FinishSpawning(SpawnTransform);

	// Launch the next projectile after the expiration of timer
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		FTimerDelegate::CreateUObject(
			this,
			&UMeteorBarrage::SpawnMeteorsOverTime, // Call the same function via creating simple TimerDelegate
			InMeteorClass,
			CurrentIndex + 1,
			InCombatTarget
		),
		FMath::RandRange(MinSpawnMeteorTime, MaxSpawnMeteorTime),
		false
	);
}

bool UMeteorBarrage::CanActivateAbility()
{
	bool bIsPlayer = GetAvatarActorFromActorInfo()->Implements<UPlayerInterface>();
	if (bIsPlayer)
	{
		if (UCustodianAbilitySystemLibrary::IsInMagicStance(Cast<ACharacter>(GetAvatarActorFromActorInfo())) && UCustodianAbilitySystemLibrary::IsAiming(Cast<ACharacter>(GetAvatarActorFromActorInfo())))
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

void UMeteorBarrage::SetMeteorSpawnLocation(FVector InLocation)
{
	MeteorCloudSpawnLocation = InLocation;
}
