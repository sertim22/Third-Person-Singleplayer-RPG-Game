// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryDataTypes.h"
#include "GameplayTagContainer.h"
#include "Inventory/WeaponInfo.h"
#include "Inventory/ArmorInfo.h"
#include "Inventory/ConsumablesInfo.h"
#include "CustodianInventoryComponent.generated.h"

class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadInventorySignature);

// Custodian Inventory Component is must be added in blueprints for all character's in game in order to implement Inventory System.
// Note: when adding this component to the Player Character use its Blueprint version (AC_InventoryComponent)
// since it has a reference to the WBP_Inventory widget blueprint
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class CUSTODIAN_API UCustodianInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCustodianInventoryComponent();

	UPROPERTY(BlueprintReadWrite, Category = "Inventory System")
	TArray<FItemMaster>ArmorAndEquipmentSlots;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory System")
	TArray<FItemMaster>ConsumablesSlots;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory System")
	int32 ConsumableItemQuantity = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory System")
	int32 ConsumableItemIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory System")
	int32 NumEquipmentSlots = 10;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory System")
	int32 NumConsumablesSlots = 7;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory System")
	float MaxWeight = 100.f;


	/*
	* Equipment
	*/

	// Data Asset containing Information about damage, damage type and other things of the weapon
	UPROPERTY(EditDefaultsOnly, Category = "Inventory System")
	UWeaponInfo* WeaponInfo;

	// Data Asset containing Information about ArmorClass, resilience and other things of the armor
	UPROPERTY(EditDefaultsOnly, Category = "Inventory System")
	UArmorInfo* ArmorInfo;

	// Data Asset containing Information about Consumables Gameplay Effects
	UPROPERTY(EditDefaultsOnly, Category = "Inventory System")
	UConsumablesInfo* ConsumablesInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Equipment")
	FGameplayTag HeadGearTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Equipment")
	FGameplayTag ChestGearTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Equipment")
	FGameplayTag ArmsGearTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Equipment")
	FGameplayTag LegsGearTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Equipment")
	FGameplayTag RightHandWeaponTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Equipment")
	FGameplayTag LeftHandWeaponTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Equipment")
	FGameplayTag RangedWeaponTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Equipment")
	FGameplayTag ConsumablesTag;

	// Armor Class of the HeadEquipmentSlot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Equipment")
	float HeadArmorClass = 0.f;

	// Armor Class of the ChestEquipmentSlot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Equipment")
	float ChestArmorClass = 0.f;

	// Armor Class of the ArmsEquipmentSlot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Equipment")
	float ArmsArmorClass = 0.f;

	// Armor Class of the LegsEquipmentSlot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Equipment")
	float LegsArmorClass = 0.f;

	/*
	* Equipment end
	*/


	/*
	* Delegates
	*/

	// Call this delegate whenever you need to refresh Inventory content (When equipping, dropping, moving items inside and etc.)
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory System|Delegates")
	FLoadInventorySignature LoadInventoryDelegate;

	// Call this delegate to update gold amount
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory System|Delegates")
	FLoadInventorySignature UpdateGoldAmountDelegate;

	// Call this delegate when picking up or dropping items
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory System|Delegates")
	FLoadInventorySignature UpdateWeightDelegate;

	// Call this delegate when equipping armor
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory System|Delegates")
	FLoadInventorySignature UpdateArmorClassDelegate;

	/*
	* Delegates end
	*/


	// Checks if overlapping with interactable actors and calls Interact event from IInventoryInterface class
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void Interact();

	// Call this to add an item to one of the slots in your inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory System", meta = (ReturnDisplayName = "AddedItem"))
	bool AddItemToInventory(FItemMaster InItemMasterData);

protected:
	
	virtual void BeginPlay() override;

	// This function is called in begin play and creates item slots.
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void SetSlotsSize();

	UPROPERTY(EditDefaultsOnly, Category = "Inventory System")
	TSubclassOf<AActor> BaseItemClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory System")
	TSubclassOf<AActor> BaseStorageClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory System")
	TObjectPtr<UDataTable> ItemDataTableClass;
};
