// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryDataTypes.h"
#include "WeaponInfo.generated.h"

// Data Asset class containing Weapon Information such as Tags, Power and etc.
UCLASS(BlueprintType)
class CUSTODIAN_API UWeaponInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// Add elements this array when creating new weapon items
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	TArray<FWeaponStats> WeaponStatsArray;

	// Call this function to get WeaponPower which is used when calculating damage
	UFUNCTION(BlueprintCallable, Category = "Weapon Stats")
	float GetWeaponPowerByTag(FGameplayTag ItemTag) const
	{
		for (const FWeaponStats& Stats : WeaponStatsArray)
		{
			if (Stats.WeaponTag == ItemTag)
			{
				return Stats.WeaponPower;
			}
		}
		return 0.f;
	}
};
