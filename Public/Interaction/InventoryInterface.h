// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "InventoryInterface.generated.h"

class UCustodianInventoryComponent;
class USkeletalMesh;
class UStaticMesh;

UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

// This interface is inherited by classes that are supposed to be a part of the interaction and inventory systems in the game
class CUSTODIAN_API IInventoryInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory System")
	void OpenInventory();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory System")
	void CloseInventory();

	// Call this event to start interacting with an actor
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System")
	void Interact(UCustodianInventoryComponent* InInventoryComponent);

	// Call this event to notify the player that the inventory has no more free slots
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory System|Inventory Widgets")
	void AddInventoryIsFullWidget();

	// Call this function to keep track of character's gold
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Gold")
	void UpdateGoldAmount(int32 InGoldAmount);


	/*
	* Setters
	*/

	// Sets MeleeWeaponTag on the CustodianCharacterBase when using an ability that requires some sort of Melee Weaponry
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Setters")
	void SetMeleeWeaponTag(FGameplayTag InMeleeWeaponTag);

	// Sets RangedWeaponTag on the CustodianCharacterBase when using an ability that requires some sort of Ranged Weaponry
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Setters")
	void SetRangedWeaponTag(FGameplayTag InRangedWeaponTag);

	// Sets ArmorClass on the CustodianCharacterBase to the value passed in
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Setters")
	void SetArmorClass(float InArmorClass);

	// Adds value to the ArmorClass variable on the CustodianCharacterBase. Can be used in Drink Potion ability and Passive Spell abilities
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Setters")
	void AddToArmorClass(float InArmorClass);

	// Sets bIsUsingConsumable variable on the CustodianCharacterBase
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Setters")
	void SetIsUsingConsumable(bool bIsUsing);

	/*
	* Setters end
	*/


	/*
	* Getters
	*/

	// Call this event to retrieve Inventory Component
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory System|Getters", meta = (ReturnDisplayName = "CustodianInventoryComponent"))
	UCustodianInventoryComponent* GetCustodianInventoryComponent();

	// Returns current character's gold amount
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Getters", meta = (ReturnDisplayName = "OutGold"))
	int32 GetCharacterGold();

	// Returns MeleeWeaponTag from the CustodianCharacterBase. Use for calculating damage
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Getters", meta = (ReturnDisplayName = "Tag"))
	FGameplayTag GetMeleeWeaponTag();

	// Returns RangedWeaponTag from the CustodianCharacterBase. Use for calculating damage
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Getters", meta = (ReturnDisplayName = "Tag"))
	FGameplayTag GetRangedWeaponTag();

	// Returns ArmorClass from the CustodianCharacterBase. Use for calculating BlockChance in "ExecCalcDamage" class
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Getters", meta = (ReturnDisplayName = "ArmorClass"))
	float GetArmorClass();

	// Returns bIsUsingConsumable variable on the CustodianCharacterBase
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Getters", meta = (ReturnDisplayName = "IsUsing"))
	bool GetIsUsingConsumable();

	/*
	* Getters end
	*/


	/*
	* Equip & Unequip Items
	*/

	// Call to load soft object reference to Head Gear Mesh and equip/unequip it
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Equipping Items")
	void EquipHeadGear(UObject* InHeadGearMesh = nullptr);

	// Call to load soft object reference to Chest Gear Mesh and equip/unequip it
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Equipping Items")
	void EquipChestGear(UObject* InChestGearMesh = nullptr);

	// Call to load soft object reference to Arms Gear Mesh and equip/unequip it
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Equipping Items")
	void EquipArmsGear(UObject* InArmsGearMesh = nullptr);

	// Call to load soft object reference to Legs Gear Mesh and equip/unequip it
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Equipping Items")
	void EquipLegsGear(UObject* InLegsGearMesh = nullptr);

	// Call to load soft object reference to RightHandMeleeWeapon Gear Mesh and equip/unequip it
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Equipping Items")
	void EquipRightHandMeleeWeapon(UObject* InRightHandMeleeWeaponMesh = nullptr);

	// Call to load soft object reference to LeftHandMeleeWeapon Gear Mesh and equip/unequip it
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Equipping Items")
	void EquipLeftHandMeleeWeapon(UObject* InLeftHandMeleeWeaponMesh = nullptr);

	// Call to load soft object reference to RangedWeapon Gear Mesh and equip/unequip it
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory System|Equipping Items")
	void EquipRangedWeaponMesh(UObject* InRangedWeaponMesh = nullptr);

	/*
	* Equip & Unequip Items end
	*/
};
