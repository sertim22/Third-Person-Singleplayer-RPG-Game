// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class USplineComponent;
class USpringArmComponent;

// The interface can only be inherited by the player character class (CustodianCharacter) and its Player Controller class (CustodianPlayerController)
class CUSTODIAN_API IPlayerInterface
{
	GENERATED_BODY()

public:

	// Call this event to save player location, progress, attributes etc.
 	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Interface|Save")
	void SaveProgress(const FName& CheckpointTag);

	// Call this event to upgrade player level
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface|Level Up")
	void LevelUp();

	/*
	* Getters
	*/

	// Returns current player level with the current amount of XP passed in
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface|Getters")
	int32 FindLevelForXP(int32 InXP) const;

	// Returns current XP of the player
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface|Getters")
	int32 GetXP() const;

	// Returns attribute points reward for the level passed in
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface|Getters")
	int32 GetAttributePointsReward(int32 Level) const;

	// Returns spell points reward for the level passed in
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface|Getters")
	int32 GetSpellPointsReward(int32 Level) const;

	// Returns current amount of attribute points
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface|Getters")
	int32 GetAttributePoints() const;

	// Returns current amount of spell points
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface|Getters")
	int32 GetSpellPoints() const;

	// Returns Spring Arm component from the player character
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface|Getters")
	USpringArmComponent* GetPlayerSpringArmComponent();

	/*
	* Getters end
	*/

	
	/*
	* Adders
	*/

	// Call this event to add XP to the player
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface|Adders")
	void AddToXP(int32 InXP);

	// Call this event to add number of level ups to the current player level. This might useful if you leveled up for several levels at one time
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface|Adders")
	void AddToPlayerLevel(int32 InPlayerLevel);

	// Call this to grant attribute points
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface|Adders")
	void AddToAttributePoints(int32 InAttributePoints);

	// Call this to grant spell points
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface|Adders")
	void AddToSpellPoints(int32 InSpellPoints);

	// Implement this function in "BP_CustodianCharacter" to let the Inventory System know that 
	// Consumable Item was used when Activating Drink Potion abilities or others
	UFUNCTION(BlueprintImplementableEvent, Category = "Player Interface|Adders")
	void ConsumableWasUsed();

	/*
	* Adders end
	*/	

	
	/*
	* Custodian Player Controller
	*/

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Interface|Custodian Player Controller")
	void PlayerEnterDefaultStance();

	/*
	* Custodian Player Controller end
	*/
};
