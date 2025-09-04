// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

// Struct containing attribute rewards and requirements
USTRUCT(BlueprintType)
struct FCustodianLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointReward = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointReward = 1;
};

// This class is used to create LevelUpInfo data asset that contains Level Up Information.
// You can define how much XP you need to get in order to upgrade the Level and also
// how many attribute and spell points will be granted after leveling up
UCLASS()
class CUSTODIAN_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// Add elements to this array to increase MAX character level
	UPROPERTY(EditDefaultsOnly)
	TArray<FCustodianLevelUpInfo> LevelUpInformation;

	// Function finds the level for the current XP passed in
	int32 FindLevelForXP(int32 XP) const;
};
