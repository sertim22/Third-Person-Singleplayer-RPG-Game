// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryDataTypes.h"
#include "ConsumablesInfo.generated.h"

class UGameplayEffect;

// Data Asset class containing All Consumable Items in the game.
// It Holds the Gameplay Effects and the Item tags use to implement Drinking
// Potion Ability and Others
UCLASS()
class CUSTODIAN_API UConsumablesInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// Add elements this array when creating new Consumables items
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumables Stats")
	TArray<FConsumableStats> ConsumablesStatsArray;

	// Call this function to get Consumables Effect Class
	UFUNCTION(BlueprintCallable, Category = "Consumables Stats")
	TSubclassOf<UGameplayEffect> GetConsumableEffectByTag(FGameplayTag ItemTag) const
	{
		for (const FConsumableStats& Stats : ConsumablesStatsArray)
		{
			if (Stats.ConsumableTag == ItemTag)
			{
				return Stats.ConsumableEffect;
			}
		}
		return nullptr;
	}

	// Call this function to get Consumables Effect Type enum variable
	UFUNCTION(BlueprintCallable, Category = "Consumables Stats")
	EEffectType GetConsumableEffectTypeByTag(FGameplayTag ItemTag) const
	{
		for (const FConsumableStats& Stats : ConsumablesStatsArray)
		{
			if (Stats.ConsumableTag == ItemTag)
			{
				return Stats.ConsumableEffectType;
			}
		}
		return EEffectType::EET_None;
	}
};
