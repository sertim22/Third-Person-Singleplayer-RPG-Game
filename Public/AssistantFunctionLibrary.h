// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryDataTypes.h"
#include "AssistantFunctionLibrary.generated.h"

class ACharacter;
class ACustodianPlayerController;
class UCustodianInventoryComponent;
class UNiagaraSystem;

// Assistan Function Library class contains helping functions to implement different actions such as loadin and unloading assets,
// getting access to the specific variables on the different classes
UCLASS()
class CUSTODIAN_API UAssistantFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*
	* Optimization
	*/

	// Loads Soft Object Pointer to the Gear Mesh and equips an item
	UFUNCTION(BlueprintCallable, Category = "Assistant Function Library|Optimization")
	static void LoadAndEquipSoftGearMesh(ACharacter* InCharacter, EEquipmentSlot InEquipmentSlot, TSoftObjectPtr<USkeletalMesh> InSoftSkeletalMesh = nullptr, TSoftObjectPtr<UStaticMesh> InSoftStaticMesh = nullptr);

	// Unloads the Gear Mesh and Unequips an item
	UFUNCTION(BlueprintCallable, Category = "Assistant Function Library|Optimization")
	static void UnloadAndUnequipSoftGearMesh(ACharacter* InCharacter, EEquipmentSlot InEquipmentSlot);

	/*
	* Optimization end
	*/


	/*
	* Setters
	*/

	// Sets bIsWieldingMeleeWeapon variable on CustodianPlayerController class to the value passed in
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	static void SetIsWieldingMeleeWeapon(ACharacter* InCharacter, bool IsWieldingMeleeWeapon);

	// Sets bIsWieldingRangedWeapon variable on CustodianPlayerController class to the value passed in
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	static void SetIsWieldingRangedWeapon(ACharacter* InCharacter, bool IsWieldingRangedWeapon);

	// Sets bIsWieldingConsumable variable on CustodianPlayerController class to the value passed in
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	static void SetIsWieldingConsumable(ACharacter* InCharacter, bool IsWieldingConsumable);

	// Sets Tags for Armor, Weapons and Consumables items that are equipped
	UFUNCTION(BlueprintCallable, Category = "Inventory System|Combat")
	static void SetEquipmentTags(UCustodianInventoryComponent* InInventoryComponent, EEquipmentSlot InItemEquipmentSlot, FGameplayTag InItemTag = FGameplayTag());

	// Call this function in "WBP_EquipmentSlot" in "UpdateSlot" event to set ArmorClass to the correct value
	UFUNCTION(BlueprintCallable, Category = "Inventory System|Combat")
	static void AddToArmorClassByTag(UCustodianInventoryComponent* InInventoryComponent, EEquipmentSlot InItemEquipmentSlot, FGameplayTag InItemTag = FGameplayTag());

	// Call this function in "WBP_EquipmentSlot" in "UpdateSlot" event to reduce ArmorClass
	UFUNCTION(BlueprintCallable, Category = "Inventory System|Combat")
	static void SubtractFromArmorClass(UCustodianInventoryComponent* InInventoryComponent, EEquipmentSlot InItemEquipmentSlot);

	/*
	* Setters end
	*/


	/*
	* Getters
	*/

	// Returns bIsWieldingMeleeWeapon variable on CustodianPlayerController 
	UFUNCTION(BlueprintCallable, Category = "Inventory System", meta = (ReturnDisplayName = "IsWielding"))
	static bool GetIsWieldingMeleeWeapon(ACharacter* InCharacter);

	// Returns bIsWieldingRangedWeapon variable on CustodianPlayerController 
	UFUNCTION(BlueprintCallable, Category = "Inventory System", meta = (ReturnDisplayName = "IsWielding"))
	static bool GetIsWieldingRangedWeapon(ACharacter* InCharacter);

	// Returns bIsWieldingConsumable variable on CustodianPlayerController 
	UFUNCTION(BlueprintCallable, Category = "Inventory System", meta = (ReturnDisplayName = "IsWielding"))
	static bool GetIsWieldingConsumable(ACharacter* InCharacter);

	// Returns the Power (Damage amount) of the weapon tag passed in
	UFUNCTION(BlueprintCallable, Category = "Inventory System", meta = (ReturnDisplayName = "OutPower"))
	static float GetWeaponPowerByTag(FGameplayTag ItemTag, UCustodianInventoryComponent* InInventoryComponent);

	// Returns ArmorClass (Value increasing chance for BlockedHit) of the Armor Item that has the tag passed in
	UFUNCTION(BlueprintCallable, Category = "Inventory System", meta = (ReturnDisplayName = "OutClass"))
	static float GetArmorClassByTag(FGameplayTag ItemTag, UCustodianInventoryComponent* InInventoryComponent);

	// Returns Consumable GameplayEffect Class of the Consumable Item that has the tag passed in
	UFUNCTION(BlueprintCallable, Category = "Inventory System", meta = (ReturnDisplayName = "OutEffect"))
	static TSubclassOf<UGameplayEffect> GetConsumableEffectClassByTag(FGameplayTag ItemTag, UCustodianInventoryComponent* InInventoryComponent);

	// Returns Consumable GameplayEffect Class of the Consumable Item that has the tag passed in
	UFUNCTION(BlueprintCallable, Category = "Inventory System", meta = (ReturnDisplayName = "OutEffectType"))
	static EEffectType GetConsumableEffectTypeByTag(FGameplayTag ItemTag, UCustodianInventoryComponent* InInventoryComponent);

	// Returns Modifier Value of the Effect Class passed in
	UFUNCTION(BlueprintCallable, Category = "Assistant Function Library", meta = (ReturnDisplayName = "OutValue"))
	static float GetGameplayEffectModifierValue(TSubclassOf<UGameplayEffect> InEffectClass, int32 Level = 1);

	// Returns Frequency Value of the Effect Class passed in
	UFUNCTION(BlueprintCallable, Category = "Assistant Function Library", meta = (ReturnDisplayName = "OutValue"))
	static float GetGameplayEffectFrequencyValue(TSubclassOf<UGameplayEffect> InEffectClass, int32 Level = 1);

	/*
	* Getters end
	*/

private:

	// Returns CustodianPlayerController variable
	static ACustodianPlayerController* GetCustodianPC(ACharacter* InCharacter);
};
