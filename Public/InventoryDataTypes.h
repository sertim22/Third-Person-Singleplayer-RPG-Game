// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InventoryDataTypes.generated.h"

// This file is a Singleton that holds different enums and structs that are used in the Inventory System

class UTexture2D;
class UStaticMesh;
class USkeletalMesh;
class UGameplayEffect;

UENUM(BlueprintType)
// Enum containing Equipment Slots names. 
enum class EEquipmentSlot : uint8
{
	EES_None UMETA(DisplayName = "None"),
	EES_Head UMETA(DisplayName = "Head"),
	EES_Chest UMETA(DisplayName = "Chest"),
	EES_Arms UMETA(DisplayName = "Arms"),
	EES_Legs UMETA(DisplayName = "Legs"),
	EES_RightHandMeleeWeapon UMETA(DisplayName = "RightHandMeleeWeapon"),
	EES_LeftHandMeleeWeapon UMETA(DisplayName = "LeftHandMeleeWeapon"),
	EES_RangedWeapon UMETA(DisplayName = "RangedWeapon"),
	EES_Consumables UMETA(DisplayName = "Consumables")
};

UENUM(BlueprintType)
// Enum holding item destination when drag and dropping it
enum class EItemDestination : uint8
{
	EID_None UMETA(DisplayName = "None"),
	EID_ItemSlot UMETA(DisplayName = "ItemSlot"),
	EID_EquipmentSlot UMETA(DisplayName = "EquipmentSlot")
};

UENUM(BlueprintType)
// Item Type enum.
enum class EItemType : uint8
{
	EIT_None UMETA(DisplayName = "None"),
	EIT_ArmorAndEquipment UMETA(DisplayName = "Armor&Equipment"),
	EIT_Consumables UMETA(DisplayName = "Consumables")
};

UENUM(BlueprintType)
// Effect Type enum.
enum class EEffectType : uint8
{
	EET_None UMETA(DisplayName = "None"),
	EET_HealthRegen UMETA(DisplayName = "Health Regeneration"),
	EET_ManaRegen UMETA(DisplayName = "Mana Regeneration")
};

// Struct containing information about an item. Create a Data Table asset in the editor based on this struct
USTRUCT(BlueprintType)
struct CUSTODIAN_API FItemDataInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Info")
	FText Name = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Info")
	FText Description = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Info")
	int32 StackSize = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Info")
	UTexture2D* ItemImage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Info")
	float ItemValue = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Info")
	TSoftObjectPtr<UStaticMesh> ItemStaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Info")
	TSoftObjectPtr<USkeletalMesh> ItemSkeletalMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Info")
	EEquipmentSlot EquipmentSlot = EEquipmentSlot::EES_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Info")
	float ItemWeight = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Info")
	FGameplayTag ItemTag;
};

// Struct that holds Item Data Table, Item Type and its Quantity
USTRUCT(BlueprintType)
struct CUSTODIAN_API FItemMaster : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Master")
	FDataTableRowHandle ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Master")
	EItemType ItemType = EItemType::EIT_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Master")
	int32 Quantity = 0;
};

// Struct containing different attributes of the weapon (its damage, damage type and etc.)
USTRUCT(BlueprintType)
struct CUSTODIAN_API FWeaponStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment Stats")
	FGameplayTag WeaponTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment Stats")
	float WeaponPower = 0.f;
};

// Struct containing different attributes of the Armor (Armor Class, Resilience and etc.)
USTRUCT(BlueprintType)
struct CUSTODIAN_API FArmorStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment Stats")
	FGameplayTag ArmorTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment Stats")
	float ArmorClass = 0.f;
};

// Struct containing Tag and GameplayEffect of the specific Potion
USTRUCT(BlueprintType)
struct CUSTODIAN_API FConsumableStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion Stats")
	FGameplayTag ConsumableTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion Stats")
	TSubclassOf<UGameplayEffect> ConsumableEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion Stats")
	EEffectType ConsumableEffectType = EEffectType::EET_None;
};
