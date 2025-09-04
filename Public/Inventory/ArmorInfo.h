// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryDataTypes.h"
#include "ArmorInfo.generated.h"

// Data Asset class containing Armor Information such as Tags, ArmorClass and etc.
UCLASS()
class CUSTODIAN_API UArmorInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// Add elements this array when creating new armor items
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor Stats")
	TArray<FArmorStats> ArmorStatsArray;

	// Call this function to get ArmorClass which is used when calculating damage
	UFUNCTION(BlueprintCallable, Category = "Armor Stats")
	float GetArmorClassByTag(FGameplayTag ItemTag) const
	{
		for (const FArmorStats& Stats : ArmorStatsArray)
		{
			if (Stats.ArmorTag == ItemTag)
			{
				return Stats.ArmorClass;
			}
		}
		return 0.f;
	}
};
