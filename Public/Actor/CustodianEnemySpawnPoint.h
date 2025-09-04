// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CustodianEnemySpawnPoint.generated.h"

class ACustodianEnemy;

UCLASS()
class CUSTODIAN_API ACustodianEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyClass")
	TSubclassOf<ACustodianEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyClass")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyClass")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
};
