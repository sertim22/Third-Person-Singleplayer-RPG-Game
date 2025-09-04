// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/Stamina/Jump.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


void UJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanActivateAbility())
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else return;
}

void UJump::Jump()
{
	ACharacter* Avatar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	Avatar->Jump();
}

void UJump::StopJump()
{
	ACharacter* Avatar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	Avatar->StopJumping();
}

bool UJump::CanActivateAbility()
{
	ACharacter* Avatar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (UCustodianAbilitySystemLibrary::IsAiming(Avatar) == false
		&& UCustodianAbilitySystemLibrary::IsEncumbered(Avatar) == false
		&& UCustodianAbilitySystemLibrary::IsBlocking(Avatar) == false
		&& UCustodianAbilitySystemLibrary::IsPreAttacking(Avatar) == false
		&& UCustodianAbilitySystemLibrary::IsAttacking(Avatar) == false
		&& Avatar->GetMovementComponent()->IsFalling() == false) return true;
	else return false;
}
