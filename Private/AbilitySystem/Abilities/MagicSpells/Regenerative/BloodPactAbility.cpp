// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/MagicSpells/Regenerative/BloodPactAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Interaction/PlayerInterface.h"
#include "GameFramework/CharacterMovementComponent.h"


FString UBloodPactAbility::GetDescription(int32 Level)
{
	float HealthCost = CalculateHealthDamage(Level);
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>Sign Blood Pact (Next Lvl)</>\n\n"
		// Level
		"<Small>Level: </><Level>%d</>\n"
		// Mana Cost
		"<Small>Health Cost: </><Damage>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		// Mana Regen and Health Damage
		"<Default>Cast this spell to summon the Spirits and sign the</> <Damage>Blood Pact</>"
		"<Default> with them. The price is </><Damage>%.1f Health Points</> <Default>but in return"
		" the Spirits fully recover your </><ManaCost>Mana Points</>"),

		Level,
		HealthCost * (-1.f),
		Cooldown,
		HealthCost* (-1.f)
	);
}

FString UBloodPactAbility::GetNextLevelDescription(int32 Level)
{
	float HealthCost = CalculateHealthDamage(Level);
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>Sign Blood Pact (Next Lvl)</>\n\n"
		// Level
		"<Small>Level: </><Level>%d</>\n"
		// Mana Cost
		"<Small>Health Cost: </><Damage>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		// Mana Regen and Health Damage
		"<Default>Cast this spell to summon the Spirits and sign the</> <Damage>Blood Pact</>"
		"<Default> with them. The price is </><Damage>%.1f Health Points</> <Default>but in return"
		" the Spirits fully recover your </><ManaCost>Mana Points</>"),

		Level,
		HealthCost * (-1.f),
		Cooldown,
		HealthCost * (-1.f)
	);
}

void UBloodPactAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanActivateAbility() && !(Cast<ACharacter>(GetAvatarActorFromActorInfo())->GetCharacterMovement()->IsFalling()))
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else return;
}

bool UBloodPactAbility::CanActivateAbility()
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

void UBloodPactAbility::ApplyBloodPact()
{
	ApplyGameplayEffectSpecToOwner(CurrentSpecHandle,
		CurrentActorInfo, CurrentActivationInfo, MakeOutgoingGameplayEffectSpec(BloodPactGameplayEffectClass));
}

float UBloodPactAbility::CalculateHealthDamage(int32 Level)
{
	if (!CostGameplayEffectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("CostGameplayEffectClass is nullptr. Please assign it in GA_BloodPact_RegenerativeSpell ability Blueprint"));
		return 0.f;
	}
	float HealthCost = 0.f;
	UGameplayEffect* HealthCostEffect = Cast<UGameplayEffect>(CostGameplayEffectClass->GetDefaultObject());
	for (const FGameplayModifierInfo& Modifier : HealthCostEffect->Modifiers)
	{
		float Magnitude = 0.f;
		Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, Magnitude);
		HealthCost += Magnitude;
	}
	return HealthCost;
}
