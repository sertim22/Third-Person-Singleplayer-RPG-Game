// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryDataTypes.h"
#include "Interaction/InventoryInterface.h"
#include "CustodianBaseItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;
class UCustodianInventoryComponent;

// This class is parent to all Items in the Custodian Project. If you wish to create a new type of item you must derive from this class
UCLASS()
class CUSTODIAN_API ACustodianBaseItem : public AActor, public IInventoryInterface
{
	GENERATED_BODY()
	
public:	
	
	ACustodianBaseItem();


	/*
	* Item Defaults
	*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Defaults")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Item Defaults")
	TObjectPtr<USphereComponent> ItemOverlapSphereCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Item Defaults")
	TObjectPtr<UWidgetComponent> InteractWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Item Defaults")
	UMaterialInstance* MI_ItemOutline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Defaults", meta = (ExposeOnSpawn = "true"))
	FItemMaster ItemMasterData;

	/*
	* Item Defaults end
	*/


	/* Inventory Interface */
	virtual void Interact_Implementation(UCustodianInventoryComponent* InInventoryComponent) override;
	/* end Inventory Interface */

protected:
	
	virtual void BeginPlay() override;

	// On ItemOverlapSphereCollision overlap function. Responsible for appearing of Outline MaterialInstance and Interact Widget
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromResult, const FHitResult& SweepResult);

	// On ItemOverlapSphereCollision overlap function. Responsible for disappearing of Outline MaterialInstance and Interact Widget
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
