// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "CustodianWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class ACustodianPlayerController;
class ACustodianPlayerState;
class UCustodianAbilitySystemComponent;
class UCustodianAttributeSet;
struct FCustodianAbilityInfo;
class UAbilityInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FCustodianAbilityInfo&, Info);

// Struct containing Player Controller and Player State, Ability System Component and Attribute Set
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

// This is parent class for all Widget Controller classes. If you want to create a new one you must derive from the class
// Widget Controller class helps with broadcasting values, its changes and etc.
UCLASS()
class CUSTODIAN_API UCustodianWidgetController : public UObject
{
	GENERATED_BODY()

public:
	
	// Sets FWidgetControllerParams struct variables to get access to the Player attributes and abilities
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	// Override this function in child classes. It broadcasts Player values such as attributes and abilities
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	virtual void BroadcastInitialValues();
	// Override this function in child classes. It broadcasts Player values when they change 
	virtual void BindCallbacksToDependencies();

	// Call this delegate to get the FCustodianAbilityInfo struct containing data about Ability
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAbilityInfoSignature AbilityInfoDelegate;

	void BroadcastAbilityInfo();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ACustodianPlayerController> CustodianPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ACustodianPlayerState> CustodianPlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UCustodianAbilitySystemComponent> CustodianAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UCustodianAttributeSet> CustodianAttributeSet;

	ACustodianPlayerController* GetCustodianPC();
	ACustodianPlayerState* GetCustodianPS();
	UCustodianAbilitySystemComponent* GetCustodianASC();
	UCustodianAttributeSet* GetCustodianAS();
};
