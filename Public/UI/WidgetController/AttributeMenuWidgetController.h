// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/CustodianWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FCustodianAttributeInfo;
struct FGameplayTag;
class UAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXPChangedSignature, float, NewXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FCustodianAttributeInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPointsChangedSignature, int32, Points);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelChangedSignature, int32, NewLevel, bool, bLevelUp);

// This class is responsible for data flow in the "Attribute Menu" widget blueprint
UCLASS(BlueprintType, Blueprintable)
class CUSTODIAN_API UAttributeMenuWidgetController : public UCustodianWidgetController
{
	GENERATED_BODY()
	
public:

	/*Custodian Widget Controller*/
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
	/*Custodian Widget Controller end*/

	// Bind event to this delegate to show Information about attributes on the screen
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	// Bind event to this delegate to show Information about AttributePoints on the screen
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "GAS|Points")
	FOnPlayerStatChangedSignature AttributePointsDelegate;

	// Bind event to this delegate to show current XP on the screen
	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnXPChangedSignature OnXPPercentChangedDelegate;

	// Bind event to this delegate to show current Player Level on the screen
	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FOnLevelChangedSignature OnPlayerLevelChangedDelegate;

	// Call this function to upgrade primary attribute
	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "GAS|Attributes")
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:

	// Shows information about the Attribute (its value, tag etc.)
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
	// Handles experience changes when OnXPPercentChangedDelegate is being called
	void OnXPChanged(int32 NewXP);
};
