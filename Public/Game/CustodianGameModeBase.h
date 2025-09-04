// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustodianGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
//class UMVVM_LoadSlot;
class USaveGame;
class UCustodianSaveGame;
class ULootTiers;

// Base Game Mode class for the Custodian project.It contains CharacterClassInfo, AbilityInfo, LootTiers data assets
// that must be set in CustodianGameModeBase blueprint. The class also saves in game progress and world state
UCLASS()
class CUSTODIAN_API ACustodianGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Loot Tiers")
	TObjectPtr<ULootTiers> LootTiers;

	//void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	
	// Returns save game from the save slot
	UCustodianSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;
	// Deletes the existing save slot
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);
	// Gets in game data from the save slot
	UCustodianSaveGame* RetrieveInGameSaveData();
	// Saves in game data and progress in save slot (abilities, items etc.)
	void SaveInGameProgressData(UCustodianSaveGame* SaveObject);
	// Saves current world state: actors, checkpoints etc.
	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	// Loads world state when loading a save slot
	void LoadWorldState(UWorld* World) const;

	//void TravelToMap(UMVVM_LoadSlot* Slot);

	UPROPERTY(EditDefaultsOnly, Category = "Save & Load")
	TSubclassOf<USaveGame> CustodianSaveGameClass;

	// Start up map name
	UPROPERTY(EditDefaultsOnly, Category = "Save & Load")
	FString DefaultMapName;

	// Start up map. Choose what level you want to start the game at
	UPROPERTY(EditDefaultsOnly, Category = "Save & Load")
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly, Category = "Save & Load")
	FName DefaultPlayerStartTag;

	// An array of maps playable in game
	UPROPERTY(EditDefaultsOnly, Category = "Save & Load")
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	// Returns Map name from the Map Asset name
	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	// Loads last world and in game data and sends player to the last checkpoint
	void PlayerDied(ACharacter* DeadCharacter);

protected:

	virtual void BeginPlay() override;
};
