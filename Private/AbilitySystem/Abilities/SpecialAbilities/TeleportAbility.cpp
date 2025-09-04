// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/SpecialAbilities/TeleportAbility.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Interaction/PlayerInterface.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


void UTeleportAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanActivateAbility())
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else return;
}

void UTeleportAbility::StartTeleport()
{
	APawn* ControlledPawn = GetAvatarActorFromActorInfo()->GetInstigatorController()->GetPawn();
	if (ControlledPawn)
	{
		// Set Character Movement to flying mode and increase fly speed
		ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
		UCharacterMovementComponent* CCMovement = ControlledCharacter->GetCharacterMovement();
		CCMovement->SetMovementMode(EMovementMode::MOVE_Flying);
		CCMovement->MaxFlySpeed = MaxTeleportSpeed;
		CCMovement->MaxAcceleration = TeleportAccelerationSpeed;

		// Make Character mesh invisible and ignore Pawn collision channel
		USkeletalMeshComponent* CCMesh = ControlledCharacter->GetMesh();
		CCMesh->SetVisibility(false, true);
		ControlledCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
}

void UTeleportAbility::EndTeleport()
{
	// Deactivate and destroy Niagara Effect
	if (TeleportNSRef)
	{
		TeleportNSRef->Deactivate();
		TeleportNSRef->DestroyComponent();
	}
	
	APawn* ControlledPawn = GetAvatarActorFromActorInfo()->GetInstigatorController()->GetPawn();
	if (ControlledPawn)
	{
		// Set Character Movement back to walking mode
		ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
		UCharacterMovementComponent* CCMovement = ControlledCharacter->GetCharacterMovement();
		CCMovement->SetMovementMode(EMovementMode::MOVE_Walking);

		// Make Character mesh visible and block Pawn collision channel
		USkeletalMeshComponent* CCMesh = ControlledCharacter->GetMesh();
		CCMesh->SetVisibility(true, true);
		ControlledCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	}
}

bool UTeleportAbility::CanActivateAbility()
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
