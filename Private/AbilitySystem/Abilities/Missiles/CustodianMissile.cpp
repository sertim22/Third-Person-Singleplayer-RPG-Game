// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/Missiles/CustodianMissile.h"
#include "Actor/Missiles/ArrowMissile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Interaction/CombatInterface.h"


void UCustodianMissile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanActivateAbility())
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else return;
}

AArrowMissile* UCustodianMissile::SpawnMissile(const FGameplayTag& SocketTag)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return nullptr;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);

	FRotator Rotation = SocketLocation.Rotation();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AArrowMissile * Missile = GetWorld()->SpawnActorDeferred<AArrowMissile>(
		ArrowMissileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Missile->SetInstigator(Cast<APawn>(GetOwningActorFromActorInfo()));
	Missile->SetOwner(GetAvatarActorFromActorInfo());
	Missile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	Missile->FinishSpawning(SpawnTransform, true);
	Missile->CollisionBox->IgnoreActorWhenMoving(GetAvatarActorFromActorInfo(), true);
	
	return Missile;
}

void UCustodianMissile::CalculateMissileDirectionAndSpeed(AArrowMissile* InMissile, float DrawStrength)
{
	if (InMissile)
	{
		InMissile->SetInstigator(Cast<APawn>(GetOwningActorFromActorInfo()));
		InMissile->SetOwner(GetAvatarActorFromActorInfo());
		InMissile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		InMissile->CollisionBox->IgnoreActorWhenMoving(GetAvatarActorFromActorInfo(), true);
		if (InMissile->MissileMovement)
		{
			InMissile->MissileMovement->Activate();
			const FVector MissileDirection = CalculateMissileDirection(InMissile);
			InMissile->CalculateMissileSpeed(MissileDirection, InMissile->MissileMovement, InMissile->MissileMovement->Velocity, DrawStrength);
		}
	}
}

void UCustodianMissile::CalculateMissileDirectionAndSpeedForAI(AArrowMissile* InMissile, float DrawStrength, const FVector& TargetLocation)
{
	if (InMissile)
	{
		InMissile->SetInstigator(Cast<APawn>(GetOwningActorFromActorInfo()));
		InMissile->SetOwner(GetAvatarActorFromActorInfo());
		InMissile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		InMissile->CollisionBox->IgnoreActorWhenMoving(GetAvatarActorFromActorInfo(), true);
		if (InMissile->MissileMovement)
		{
			InMissile->MissileMovement->Activate();
			FVector MissileDirection = (TargetLocation - InMissile->GetActorLocation()).GetSafeNormal();
			InMissile->CalculateMissileSpeed(MissileDirection, InMissile->MissileMovement, InMissile->MissileMovement->Velocity, DrawStrength);
		}
	}
}

FVector UCustodianMissile::CalculateMissileDirection(AArrowMissile* InMissile)
{
	UCameraComponent* Camera = GetAvatarActorFromActorInfo()->GetComponentByClass<UCameraComponent>();
	const FVector Start = Camera->K2_GetComponentLocation();
	const FVector End = Camera->GetForwardVector() * 10000.f + Start;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(InMissile);
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
	const FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(InMissile->GetActorLocation(), SelectedVector);
	return Direction;
}

bool UCustodianMissile::CanActivateAbility()
{
	if (GetAvatarActorFromActorInfo()->Implements<UPlayerInterface>())
	{
		if (UCustodianAbilitySystemLibrary::IsInRangedStance(Cast<ACharacter>(GetAvatarActorFromActorInfo()))
			&& UCustodianAbilitySystemLibrary::IsAiming(Cast<ACharacter>(GetAvatarActorFromActorInfo()))
			&& ICombatInterface::Execute_CanDrawRanged(GetAvatarActorFromActorInfo())
			&& ICombatInterface::Execute_GetIsDrawing(GetAvatarActorFromActorInfo())) return true;
		else return false;
	}
	else
	{
		return true;
	}
}
