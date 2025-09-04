// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/MagicSpells/Regenerative/SpawnAuraAbility.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Interaction/PlayerInterface.h"
#include "Actor/CustodianEffectActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"


void USpawnAuraAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanActivateAbility() && !(Cast<ACharacter>(GetAvatarActorFromActorInfo())->GetCharacterMovement()->IsFalling())) 
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else return;
}

ACustodianEffectActor* USpawnAuraAbility::SpawnAura()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return nullptr;

	// DO a line trace to find a spawn location for the Aura Actor
	FVector SpawnLocation = FVector();
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,
		Cast<ACharacter>(GetAvatarActorFromActorInfo())->GetMesh()->GetComponentLocation(),
		FVector(0.f, 0.f, -AuraZOffset),
		ECollisionChannel::ECC_Visibility);
	if (bHit == true)
	{
		SpawnLocation = HitResult.ImpactPoint;
	}
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);

	ACustodianEffectActor* SpawnedAura = GetWorld()->SpawnActorDeferred<ACustodianEffectActor>(
		EffectActorClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	SpawnedAura->SetInstigator(Cast<APawn>(GetOwningActorFromActorInfo()));
	SpawnedAura->SetOwner(GetAvatarActorFromActorInfo());
	SpawnedAura->FinishSpawning(SpawnTransform);

	if (SpawnedAura) return SpawnedAura;
	else return nullptr;
}

bool USpawnAuraAbility::CanActivateAbility()
{
	bool bIsPlayer = GetAvatarActorFromActorInfo()->Implements<UPlayerInterface>();
	if (bIsPlayer)
	{
		if (UCustodianAbilitySystemLibrary::IsInMagicStance(Cast<ACharacter>(GetAvatarActorFromActorInfo()))
			&& UCustodianAbilitySystemLibrary::IsAiming(Cast<ACharacter>(GetAvatarActorFromActorInfo())) == false)
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
