// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "CustodianInputConfig.generated.h"

// Struct containing InputAction and Input Tag variables that must be set in blueprints
USTRUCT(BlueprintType)
struct FCustodianInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

// Use this class to create CustodianInputConfig Data Asset to add Input Actions for the player abilites.
// If you do not add Input Action to this data asset the ability you created won't activate!
UCLASS()
class CUSTODIAN_API UCustodianInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	// Array that contains all input actions for the Player Character abilities.
	// Don't forget to add Input Action here after creating a new ability with new Input Tag!
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCustodianInputAction> AbilityInputActions;
};
