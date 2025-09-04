// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/CustodianWidgetController.h"
#include "OverlayWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UCustodianUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

class UCustodianUserWidget;
class UAbilityInfo;
class UCustodianAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterStanceChangedSignature, ECharacterStance, CharacterStance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryStateSignature, bool, bIsInventoryOpened);

// This class is responsible for data flow in the "WBP_Overlay" widget blueprint.
// Here vital attributes are shown on screen and Inventory Widget.
UCLASS(BlueprintType, Blueprintable)
class CUSTODIAN_API UOverlayWidgetController : public UCustodianWidgetController
{
	GENERATED_BODY()
	
public:

	/*Custodian Widget Controller*/
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	/*Custodian Widget Controller end*/

	// Bind event to this delegate to show current health
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	// Bind event to this delegate to update current health
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	// Bind event to this delegate to show current stamina
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnStaminaChanged;

	// Bind event to this delegate to update current stamina
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxStaminaChanged;

	// Bind event to this delegate to show current mana
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	// Bind event to this delegate to update current mana
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	// Bind event to this delegate to show current Character Stance
	UPROPERTY(BlueprintAssignable, Category = "Character Defaults")
	FOnCharacterStanceChangedSignature OnCharacterStanceChangedDelegate;

	// Bind event to this delegate to show or hide the Inventory Widget blueprint
	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnInventoryStateSignature OnInventoryStateDelegate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot) const;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	{
		return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	}
}

