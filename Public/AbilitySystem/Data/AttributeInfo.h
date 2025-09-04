// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

// Struct containing attribute name, description and tag
USTRUCT(BlueprintType)
struct FCustodianAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;

};

// This class has the names, descriptions and tags of all attributes in game.
// Use it to create a AttributeInfo data asset to visually represent your attributes in widget blueprints.
UCLASS()
class CUSTODIAN_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// Finds a specific attribute info by passing in the tag
	FCustodianAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	// Add elements to this array to add more attributes information for your existing attributes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttributeInformation")
	TArray<FCustodianAttributeInfo> AttributeInformation;
};
