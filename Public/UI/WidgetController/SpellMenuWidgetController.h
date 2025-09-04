// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/CustodianWidgetController.h"
#include "CustodianGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellButtonSelectedSignature, bool, bSpendPointButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellButtonReassignedSignature, const FGameplayTag&, AbilityTag);

// Struct containing Ability Tag and Its status (Is it FireBolt, Is this ability Equipped or Eligible? Etc.)
struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

// This class is responsible for data flow in the "Spell Menu" widget blueprint
UCLASS(BlueprintType, Blueprintable)
class CUSTODIAN_API USpellMenuWidgetController : public UCustodianWidgetController
{
	GENERATED_BODY()
	
public:

	// Custodian Widget Controller
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
	// Custodian Widget Controller end

	// Bind event to this delegate in "WBP_SpellMenu" to broadcast current spell points value
	UPROPERTY(BlueprintAssignable, Category = "GAS|Points")
	FOnPlayerStatChangedSignature SpellPointsDelegate;

	// Bind event to this delegate in "WBP_SpellMenu" to Enable "Spend Point" and "Equip"
	// Buttons based on Ability Status and Spell Points. And also to Show Spell description
	UPROPERTY(BlueprintAssignable, Category = "Spell Menu")
	FSpellButtonSelectedSignature SpellButtonSelectedDelegate;

	// Bind event to this delegate in "WBP_SpellMenu" to play the correct animation in the "WBP_EquippedSpellRow" widget
	UPROPERTY(BlueprintAssignable, Category = "Spell Menu")
	FWaitForEquipSelectionSignature WaitForEquipDelegate;

	// Bind event to this delegate in "WBP_SpellMenu" to stop playing all animations and play "Hide" animations
	UPROPERTY(BlueprintAssignable, Category = "Spell Menu")
	FWaitForEquipSelectionSignature StopWaitingForEquipDelegate;

	// Bind event to this delegate in "WBP_SpellButton" to play sound and hide "Selection_Image"
	UPROPERTY(BlueprintAssignable, Category = "Spell Menu")
	FSpellButtonReassignedSignature SpellButtonReassignedDelegate;

	// Call this function when selecting any Spell to broadcast "SpellButtonSelectedDelegate"
	// containing information about whether "Equip" and "Spend Point" should be enabled
	// and Descriptions of the selected ability
	UFUNCTION(BlueprintCallable, Category = "Spell Menu")
	void SpellButtonSelected(const FGameplayTag& AbilityTag);

	// Call this function in "WBP_SpellMenu" when pressing "Spend Point" button.
	// It calls its Server version to either Unlock or Upgrade selected Ability
	UFUNCTION(BlueprintCallable, Category = "Spell Menu")
	void SpendSpellPointButtonPressed();

	// Call this function when deselecting Spell to stop broadcasting information about
	// previously selected ability and Disable "Equip" and "Spend Point" buttons
	UFUNCTION(BlueprintCallable, Category = "Spell Menu")
	void SpellButtonDeselect();

	// Call this function when pressing the "Equip" button to broadcast "WaitForEquipDelegate"
	// value and send the Ability Type of the selected Spell to play the correct "Selection" Animation
	UFUNCTION(BlueprintCallable, Category = "Spell Menu")
	void EquipButtonPressed();

	// Call this function to equip the Spell in the right Spell Row.
	// It also calls its Server version which broadcasts Ability Status of the spell
	// and enables or disables Niagara Effects for Passive Spells if it's being equipped or unequipped
	UFUNCTION(BlueprintCallable, Category = "Spell Menu")
	void SpellRowButtonPressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

	// Bind this callback to the "AbilityEquipped" delegate from the CustodianAbilitySystemComponent class
	// to broadcast AbilityInfo (InputTag, StatusTag) of the Equipped Spell ability
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot);

private:

	// This function defines whether "Equip" and "SpendPoint" buttons should be enabled or not.
	// It checks against AbilityType, Status and amount of SpellPoints 
	static void ShouldEnableButtons(const FGameplayTag AbilityType, const FGameplayTag AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpendPointButton, bool& bShouldEnableEquipButton);
	// Variable containing Ability Tag and Its status (Is it FireBolt, Is this ability Equipped or Eligible? Etc.)
	FSelectedAbility SelectedAbility = { FCustodianGameplayTags::Get().Abilities_None, FCustodianGameplayTags::Get().Abilities_Status_Locked };
	// Variable that keeps track of how many available Spell Points does the player possess 
	int32 CurrentSpellPoints = 0;
	// Variable that defines whether Selected Spell being equipped or not
	bool bWaitingForEquipSelection = false;
	// Variable that stores the Input Tag of the EquippedRowButton slot
	FGameplayTag SelectedSlot;
};
