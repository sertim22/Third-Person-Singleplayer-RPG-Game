// Copyright Sertim all rights reserved


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/CustodianAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/CustodianPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	//BroadcastAbilityInfo();

	SpellPointsDelegate.Broadcast(GetCustodianPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetCustodianASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				// Get Ability Type of the Selected Spell
				const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;

				SelectedAbility.Status = StatusTag;
				bool bEnableSpendPoint = false;
				bool bEnableEquip = false;
				ShouldEnableButtons(SelectedAbilityType, StatusTag, CurrentSpellPoints, bEnableSpendPoint, bEnableEquip);
				FString Description;
				FString NextLevelDescription;
				GetCustodianASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
				SpellButtonSelectedDelegate.Broadcast(bEnableSpendPoint, bEnableEquip, Description, NextLevelDescription);
			}

			if (AbilityInfo)
			{
				FCustodianAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);

	GetCustodianASC()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

	GetCustodianPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 SpellPoints)
		{
			// Get Ability Type of the Selected Spell
			const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

			SpellPointsDelegate.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;

			bool bEnableSpendPoint = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(SelectedAbilityType, SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoint, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetCustodianASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
			SpellButtonSelectedDelegate.Broadcast(bEnableSpendPoint, bEnableEquip, Description, NextLevelDescription);
		}
	);
}

void USpellMenuWidgetController::SpellButtonSelected(const FGameplayTag& AbilityTag)
{
	// Get Ability Type of the Selected Spell
 	const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;

	if (bWaitingForEquipSelection)
	{
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	const FCustodianGameplayTags GameplayTags = FCustodianGameplayTags::Get();
	const int32 SpellPoints = GetCustodianPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetCustodianASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetCustodianASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnableSpendPoint = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(SelectedAbilityType, AbilityStatus, SpellPoints, bEnableSpendPoint, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetCustodianASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellButtonSelectedDelegate.Broadcast(bEnableSpendPoint, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendSpellPointButtonPressed()
{
	if (GetCustodianASC())
	{
		GetCustodianASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellButtonDeselect()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	SelectedAbility.Ability = FCustodianGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FCustodianGameplayTags::Get().Abilities_Status_Locked;
	SpellButtonSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus = GetCustodianASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FCustodianGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetCustodianASC()->GetSlotFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellRowButtonPressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection) return;
	// Check Selected Ability against the slot's Ability Type 
	// (don't equip an offensive spell in a passive or regenerative slot and vice versa)
	const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetCustodianASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot)
{
	bWaitingForEquipSelection = false;

	const FCustodianGameplayTags& GameplayTags = FCustodianGameplayTags::Get();

	FCustodianAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PrevSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty Info if Previous Slot is a valid slot. Only if equipping an already-equipped spell.
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// Fill in the new slot.
	FCustodianAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	SpellButtonReassignedDelegate.Broadcast(AbilityTag);
	SpellButtonDeselect();
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag AbilityType, const FGameplayTag AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpendPointButton, bool& bShouldEnableEquipButton)
{
	const FCustodianGameplayTags GameplayTags = FCustodianGameplayTags::Get();

	bShouldEnableSpendPointButton = false;
	bShouldEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0 && AbilityType != GameplayTags.Abilities_Type_Passive)
		{
			bShouldEnableSpendPointButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0 && AbilityType != GameplayTags.Abilities_Type_Passive)
		{
			bShouldEnableSpendPointButton = true;
		}
	}
}
