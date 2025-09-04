// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/MagicSpells/Offensive/SpellTrap.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Interaction/PlayerInterface.h"
#include "Interaction/EnemyInterface.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actor/CustodianEffectActor.h"


void USpellTrap::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanActivateAbility())
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else return;
}

bool USpellTrap::CanActivateAbility()
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

FVector USpellTrap::CalculateTrapSpawnLocation()
{
	// For Player
	if (GetAvatarActorFromActorInfo()->Implements<UPlayerInterface>())
	{
		UCameraComponent* Camera = GetAvatarActorFromActorInfo()->GetComponentByClass<UCameraComponent>();
		const FVector Start = Camera->K2_GetComponentLocation();
		const FVector End = Camera->GetForwardVector() * MaxSpawnDistance + Start;

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
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
		return SelectedVector;
	}
	// For AI
	else
	{
		FVector SpawnLocation = FVector();
		AActor* CombatTarget = IEnemyInterface::Execute_GetCombatTarget(GetAvatarActorFromActorInfo());
		if (CombatTarget)
		{
			FHitResult HitResult;
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,
				CombatTarget->GetActorLocation(),
				FVector(0.f, 0.f, -MaxSpawnDistance),
				ECollisionChannel::ECC_Visibility);
			if (bHit == true)
			{
				SpawnLocation = HitResult.ImpactPoint;
			}
			return SpawnLocation;
		}
		else return SpawnLocation;
	}
}

ACustodianEffectActor* USpellTrap::SpawnSpellTrap()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return nullptr;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(CalculateTrapSpawnLocation());

	ACustodianEffectActor* SpawnedTrap = GetWorld()->SpawnActorDeferred<ACustodianEffectActor>(
		EffectActorClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	SpawnedTrap->SetInstigator(Cast<APawn>(GetOwningActorFromActorInfo()));
	SpawnedTrap->SetOwner(GetAvatarActorFromActorInfo());
	SpawnedTrap->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	SpawnedTrap->FinishSpawning(SpawnTransform);

	return SpawnedTrap;
}
