// Copyright Sertim all rights reserved


#include "Inventory/CustodianInventoryComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/InventoryInterface.h"


UCustodianInventoryComponent::UCustodianInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCustodianInventoryComponent::Interact()
{
	check(BaseItemClass);
	check(BaseStorageClass);

	ACharacter* ICOwner = Cast<ACharacter>(GetOwner());
	if (!ICOwner) return;

	TArray<AActor*> OverlappingActors;
	ICOwner->GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->IsA(BaseItemClass) || OverlappingActor->IsA(BaseStorageClass))
		{
			IInventoryInterface::Execute_Interact(OverlappingActor, this);
		}
	}
}

bool UCustodianInventoryComponent::AddItemToInventory(FItemMaster InItemMasterData)
{
	switch (InItemMasterData.ItemType)
	{
	case EItemType::EIT_ArmorAndEquipment:
		for (int32 Index = 0; Index < ArmorAndEquipmentSlots.Num(); ++Index)
		{
			// AaESlot = ArmorAndEquipmentSlot
			FItemMaster AaESlot = ArmorAndEquipmentSlots[Index];
			if (AaESlot.Quantity == 0)
			{
				ArmorAndEquipmentSlots[Index] = InItemMasterData;
				return true;
			}
		}
		return false;
		break;

	case EItemType::EIT_Consumables:
		for (int32 Index = 0; Index < ConsumablesSlots.Num(); ++Index)
		{
			// CSlot = ConsumablesSlot
			FItemMaster CSlot = ConsumablesSlots[Index];
			int32 CIQuantity = CSlot.Quantity;
			if (CSlot.ItemDataTable.RowName == InItemMasterData.ItemDataTable.RowName)
			{
				FItemDataInfo* OutRow = ItemDataTableClass->FindRow<FItemDataInfo>(InItemMasterData.ItemDataTable.RowName, TEXT(""));
				if (OutRow->StackSize >= CIQuantity + InItemMasterData.Quantity)
				{
					FItemMaster ItemMaster;
				    ItemMaster.Quantity = CIQuantity + InItemMasterData.Quantity;
					ItemMaster.ItemDataTable = InItemMasterData.ItemDataTable;
					ItemMaster.ItemType = InItemMasterData.ItemType;

					ConsumablesSlots[Index] = ItemMaster;
					return true;
				}
			}
		}
		for (int32 Index = 0; Index < ConsumablesSlots.Num(); ++Index)
		{
			// CSlot = ConsumablesSlot
			FItemMaster CSlot = ConsumablesSlots[Index];
			if (CSlot.Quantity == 0)
			{
				ConsumablesSlots[Index] = InItemMasterData;
				return true;
			}
		}
		return false;
		break;

	default:
		UE_LOG(LogTemp, Error, TEXT("Unknown Item!"));
		break;
	}
	return false;
}

void UCustodianInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	SetSlotsSize();
}

void UCustodianInventoryComponent::SetSlotsSize()
{
	ArmorAndEquipmentSlots.SetNum(NumEquipmentSlots);
	ConsumablesSlots.SetNum(NumConsumablesSlots);
}

