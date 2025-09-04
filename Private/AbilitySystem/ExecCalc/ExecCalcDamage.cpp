// Copyright Sertim all rights reserved


#include "AbilitySystem/ExecCalc/ExecCalcDamage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/CustodianAttributeSet.h"
#include "CustodianGameplayTags.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/InventoryInterface.h"
#include "Kismet/GameplayStatics.h"
#include "CustodianAbilityTypes.h"
#include "AssistantFunctionLibrary.h"
#include "Inventory/CustodianInventoryComponent.h"


struct CustodianDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	CustodianDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustodianAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustodianAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustodianAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustodianAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustodianAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustodianAttributeSet, CriticalHitResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustodianAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustodianAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustodianAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustodianAttributeSet, PhysicalResistance, Target, false);
	}
};

static const CustodianDamageStatics& DamageStatics()
{
	static CustodianDamageStatics DStatics;
	return DStatics;
}

UExecCalcDamage::UExecCalcDamage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalcDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FCustodianGameplayTags& Tags = FCustodianGameplayTags::Get();

	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);

	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourceCharacterLevel = 1;
	if (SourceAvatar && SourceAvatar->Implements<UCombatInterface>())
	{
		SourceCharacterLevel = ICombatInterface::Execute_GetCharacterLevel(SourceAvatar);
	}
	int32 TargetCharacterLevel = 1;
	if (TargetAvatar && TargetAvatar->Implements<UCombatInterface>())
	{
		TargetCharacterLevel = ICombatInterface::Execute_GetCharacterLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetEffectContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Determine Debuff IF the TargetAvatar is NOT immune to them
	if (ICombatInterface::Execute_GetIsIgnoringDebuffs(TargetAvatar) == false)
	{
		DetermineDebuff(Spec, ExecutionParams, EvaluationParameters, TagsToCaptureDefs);
	}

	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	// Check if should add to Damage "Weapon Power" of the equipped item
	if (UCustodianAbilitySystemLibrary::IsInMeleeStance(Cast<ACharacter>(SourceAvatar))
		|| UCustodianAbilitySystemLibrary::IsInRangedStance(Cast<ACharacter>(SourceAvatar)))
	{
		Damage = FindAvatarWeaponPower(SourceAvatar);
	}

	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FCustodianGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn`t contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);
		if (DamageTypeValue <= 0.f)
		{
			continue;
		}

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;

		if (UCustodianAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			// 1. override TakeDamage in CustodianCharacterBase. *
			// 2. create delegate OnDamageDelegate, broadcast recieved in TakeDamage. *
			// 3. bind Lambda to OnDamageDelegate on the Victim here. *
			// 4. Call UGameplayStatics::ApplyRadialDamageWithFalloff to cause damage (this will result in TakeDamage being called
			//    on the victim, which will then broadcast OnDamageDelegate). *
			// 5. In Lambda, set DamageTypeValue to the damage recieved from the broadcast. *

			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				CombatInterface->GetOnDamageDelegate().AddLambda([&](float DamageAmount)
					{
						DamageTypeValue = DamageAmount;
					});
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff(TargetAvatar,
				DamageTypeValue,
				0.f,
				UCustodianAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UCustodianAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UCustodianAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr);
		}

		Damage += DamageTypeValue;
	}


	/*
	* Blocked Hit
	*/

	// Capture BlockChance on Target, and determine if there was a successful BlockHit.
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	const bool bBlockedHit = FMath::RandRange(1, 100) < TargetBlockChance + FindTargetArmorClass(TargetAvatar);
	UCustodianAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlockedHit);
	// If BlockedHit halve the damage.
	Damage = bBlockedHit ? Damage = Damage * 0.5f : Damage;

	/*
	* Blocked Hit end
	*/


	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const UCharacterClassInfo* CharacterClassInfo = UCustodianAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficents->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficent = ArmorPenetrationCurve->Eval(SourceCharacterLevel);

	//Armor Penetration ignores percentage of the Target`s Armor.
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficent) / 100.f;

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficents->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficent = EffectiveArmorCurve->Eval(TargetCharacterLevel);
	// Armor Ignores a percentage of Incoming Damage.
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficent) / 100.f;

	// Capture CriticalHitChance on Source, CriticalHitResistance on Target and CriticalHitDamage Bonus on Source
	float SourceCritHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCritHitChance);
	SourceCritHitChance = FMath::Max<float>(SourceCritHitChance, 0.f);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	float CriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, CriticalHitDamage);
	CriticalHitDamage = FMath::Max<float>(CriticalHitDamage, 0.f);

	// Critical Hit Resistance Coefficent Curve
	const FRealCurve* CritHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficents->FindCurve(FName("CriticalHitResistance"), FString());
	const float CritHitResCoefficent = CritHitResistanceCurve->Eval(TargetCharacterLevel);

	// Determine if there was a successful Critical Hit.
	const float EffectiveCriticalHitChance = SourceCritHitChance - TargetCriticalHitResistance * CritHitResCoefficent;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	UCustodianAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	// If CriticalHit, double the damage + add CriticalHitDamage bonus.
	Damage = bCriticalHit ? Damage * 2.f + CriticalHitDamage : Damage;

	const FGameplayModifierEvaluatedData EvaluatedData(UCustodianAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UExecCalcDamage::DetermineDebuff(const FGameplayEffectSpec& Spec, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters& EvaluationParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDefs) const
{
	const FCustodianGameplayTags GameplayTags = FCustodianGameplayTags::Get();

	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if (TypeDamage > -.5f) // .5 padding for floating point [im]precision
		{
			// Determine if there was a successful debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToCaptureDefs[ResistanceTag], EvaluationParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				UCustodianAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				UCustodianAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, -1.f);

				UCustodianAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UCustodianAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UCustodianAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}

float UExecCalcDamage::FindAvatarWeaponPower(AActor* InSourceAvatar) const
{
	if (InSourceAvatar)
	{
		UCustodianInventoryComponent* IC = IInventoryInterface::Execute_GetCustodianInventoryComponent(InSourceAvatar);
		// Calculate for Ranged Weapon damage
		if (ICombatInterface::Execute_GetShouldApplyRangedDamage(InSourceAvatar) == true)
		{
			FGameplayTag RangedWeaponTag = IInventoryInterface::Execute_GetRangedWeaponTag(InSourceAvatar);
			return UAssistantFunctionLibrary::GetWeaponPowerByTag(RangedWeaponTag, IC) * ICombatInterface::Execute_GetRangedDamageMultiplier(InSourceAvatar);
		}
		// Calculate for Melee Weapon damage
		else
		{
			FGameplayTag MeleeWeaponTag = IInventoryInterface::Execute_GetMeleeWeaponTag(InSourceAvatar);
			return UAssistantFunctionLibrary::GetWeaponPowerByTag(MeleeWeaponTag, IC);
		}
	}
	else return 0.f;
}

float UExecCalcDamage::FindTargetArmorClass(AActor* InTargetAvatar) const
{
	if (InTargetAvatar)
	{
		return IInventoryInterface::Execute_GetArmorClass(InTargetAvatar);
	}
	else return 0.f;
}
