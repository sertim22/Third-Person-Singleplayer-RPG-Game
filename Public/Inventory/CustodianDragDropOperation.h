// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDataTypes.h"
#include "CustodianDragDropOperation.generated.h"

// This Drag & Drop Operation class must be used in all widgets connected to the Inventory System and Items in game in order to get relevant information
// from ItemDataTableInfo created in blueprints. 
UCLASS()
class CUSTODIAN_API UCustodianDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	// Index represents position of the item slot in the inventory.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory System", meta = (ExposeOnSpawn = "true"))
	int32 ItemIndex = 0;

	// This variable holds information about Item Data Table, Item Type and its quantity
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory System", meta = (ExposeOnSpawn = "true"))
	FItemMaster ItemMaster;

	// This enum keeps information wheter the item is being dragged onto Equipment or Item slot. (whether the item is being put on or taken off)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory System", meta = (ExposeOnSpawn = "true"))
	EItemDestination ItemDestination;
};
