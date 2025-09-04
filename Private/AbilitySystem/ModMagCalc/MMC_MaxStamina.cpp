// Copyright Sertim all rights reserved


#include "AbilitySystem/ModMagCalc/MMC_MaxStamina.h"
#include "AbilitySystem/CustodianAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxStamina::UMMC_MaxStamina()
{
	EnduranceDef.AttributeToCapture = UCustodianAttributeSet::GetConstitutionAttribute();
	EnduranceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	EnduranceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(EnduranceDef);
}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather gameplay source and target tags
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Endurance = 0.f;
	GetCapturedAttributeMagnitude(EnduranceDef, Spec, EvaluationParameters, Endurance);
	Endurance = FMath::Max<float>(Endurance, 0.f);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}

	return 80.f + 2.5 * Endurance + 10.f * PlayerLevel;
}
