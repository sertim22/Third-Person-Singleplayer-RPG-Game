// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/Stamina/Sprint.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void USprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanActivateAbility())
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else return;
}

void USprint::StartSprint()
{
	ACharacter* Avatar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	Avatar->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void USprint::StopSprint()
{
	ACharacter* Avatar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	Avatar->GetCharacterMovement()->MaxWalkSpeed = JoggingSpeed;
}

bool USprint::CanActivateAbility()
{
	ACharacter* Avatar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (UCustodianAbilitySystemLibrary::IsAiming(Avatar) == false
		&& UCustodianAbilitySystemLibrary::IsEncumbered(Avatar) == false
		&& UCustodianAbilitySystemLibrary::IsBlocking(Avatar) == false
		&& UCustodianAbilitySystemLibrary::IsPreAttacking(Avatar) == false
		&& UCustodianAbilitySystemLibrary::IsAttacking(Avatar) == false
		&& Avatar->GetMovementComponent()->IsFalling() == false
		&& UKismetMathLibrary::VSizeXY(Avatar->GetCharacterMovement()->Velocity) > 3.f) return true;
	else return false;
}
