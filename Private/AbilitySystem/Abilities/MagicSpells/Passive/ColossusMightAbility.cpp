// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/MagicSpells/Passive/ColossusMightAbility.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Interaction/PlayerInterface.h"
#include "GameFramework/SpringArmComponent.h"


FString UColossusMightAbility::GetDescription(int32 Level)
{
	const int32 StrengthUpgrade = CalculateStrengthUpgrade(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>Might of the Colossus</>\n\n"

		"<Default>Assign this Passive Spell in the Spell Slot. It will increase "
		"the Size of Your Character and </><Attribute>Strength by %d</>. "),

		// Values
		StrengthUpgrade);
}

FString UColossusMightAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Passive Spells cannot be upgraded.</>"));
}

void UColossusMightAbility::EnableCollossusMight()
{
   // Increase Character's Strength attribute
    if (EnableColossusMightEffectClass)
    {
        ApplyGameplayEffectSpecToOwner(CurrentSpecHandle,
            CurrentActorInfo, CurrentActivationInfo, MakeOutgoingGameplayEffectSpec(EnableColossusMightEffectClass));

        ACharacter* Avatar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
        // Set Intial Character's size before changing anything
        InitalCapsuleRadius = Avatar->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
        InitalCapsuleHalfHeight = Avatar->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
        InitialMeshZOffset = Avatar->GetMesh()->GetRelativeLocation().Z;
        IntialMeshScale = Avatar->GetMesh()->GetRelativeScale3D();

        // Increasse Character's size (Capsule Component and Mesh and Change ZOffset of the Mesh)
        Avatar->GetCapsuleComponent()->SetCapsuleSize(IncreasedCapsuleRadius, IncreasedCapsuleHalfHeight);
        Avatar->GetMesh()->SetRelativeScale3D(FVector(IncreasedMeshScale));
        Avatar->GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, ChangedMeshZOffset));

        // Change SpringArm location if the Avatar is a Player Character
        if (Avatar->Implements<UPlayerInterface>())
        {
            USpringArmComponent* PlayerSpringArm = IPlayerInterface::Execute_GetPlayerSpringArmComponent(Avatar);
            // Save Initial Target Arm Length and Its ZOffset
            InitialSpringArmTargetLength = PlayerSpringArm->TargetArmLength;
            InitialSpringArmZOffset = PlayerSpringArm->GetRelativeLocation().Z;
            // Change Spring Arm position
            PlayerSpringArm->TargetArmLength = ChangedSpringArmTargetLength;
            PlayerSpringArm->SetRelativeLocation(FVector(0.f, 0.f, ChangedSpringArmZOffset));
        }
    }
}

void UColossusMightAbility::DisableCollossusMight()
{
    // Decrease Character's Strength attribute
    if (DisableColossusMightEffectClass)
    {
        ApplyGameplayEffectSpecToOwner(CurrentSpecHandle,
            CurrentActorInfo, CurrentActivationInfo, MakeOutgoingGameplayEffectSpec(DisableColossusMightEffectClass));

        // Decrease Character's size (Capsule Component and Change ZOffset of the Mesh)
        ACharacter* Avatar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
        Avatar->GetCapsuleComponent()->SetCapsuleSize(InitalCapsuleRadius, InitalCapsuleHalfHeight);
        Avatar->GetMesh()->SetRelativeScale3D(IntialMeshScale);
        Avatar->GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, InitialMeshZOffset));

        // Change SpringArm location if the Avatar is a Player Character
        if (Avatar->Implements<UPlayerInterface>())
        {
            USpringArmComponent* PlayerSpringArm = IPlayerInterface::Execute_GetPlayerSpringArmComponent(Avatar);
            // Set Spring Arm back to Initial position
            PlayerSpringArm->TargetArmLength = InitialSpringArmTargetLength;
            PlayerSpringArm->SetRelativeLocation(FVector(0.f, 0.f, InitialSpringArmZOffset));
        }
    }
}

float UColossusMightAbility::CalculateStrengthUpgrade(int32 Level)
{
    if (!EnableColossusMightEffectClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ColossusMightEffectClass is nullptr. Please assign it in GA_ColossusMight_PassiveSpell Blueprint"));
        return 0.f;
    }
    float StrengthUpgrade = 0.f;
    UGameplayEffect* MightEffect = Cast<UGameplayEffect>(EnableColossusMightEffectClass->GetDefaultObject());
    for (const FGameplayModifierInfo& Modifier : MightEffect->Modifiers)
    {
        float Magnitude = 0.f;
        Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, Magnitude);
        StrengthUpgrade += Magnitude;
    }
    return StrengthUpgrade;
}
