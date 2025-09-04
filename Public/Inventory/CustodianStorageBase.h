// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InventoryInterface.h"
#include "InventoryDataTypes.h"
#include "CustodianStorageBase.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;
class UCustodianInventoryComponent;

// This class is parent to all Storage objects such as Chests, Boxes, Barrows etc.
// If you wish to create a new one you need to derive from this class
UCLASS()
class CUSTODIAN_API ACustodianStorageBase : public AActor, public IInventoryInterface
{
	GENERATED_BODY()
	
public:	
	
	ACustodianStorageBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Storage Defaults")
	TObjectPtr<UStaticMeshComponent> StorageMesh;

	// Component around which MovableStorageMesh will rotate 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Storage Defaults")
	TObjectPtr<USceneComponent> RotationComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Storage Defaults")
	TObjectPtr<UStaticMeshComponent> MovableStorageMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Storage Defaults")
	TObjectPtr<USphereComponent> StorageOverlapSphereCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Storage Defaults")
	TObjectPtr<UWidgetComponent> InteractWidget;

	// An array of items located in the storage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage Defaults", meta = (ExposeOnSpawn = "true"))
	TArray<FItemMaster> ItemsInStorage;

	virtual void Interact_Implementation(UCustodianInventoryComponent* InInventoryComponent) override;

	// Call this event to trigger interaction with the storage
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory System|Storage")
	void StorageInteract();

	// Removes an item from the storage when picking it up or dropping it
	UFUNCTION(BlueprintCallable, Category = "Inventory System|Storage")
	void RemoveItemFromStorage(int32 ItemIndex);

protected:
	
	virtual void BeginPlay() override;

	// Creates interact widget and shows outline material instance OnBeginOverlap
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromResult, const FHitResult& SweepResult);

	// Deletes interact widget and hides outline material instance OnEndOverlap
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category = "Storage Defaults")
	UMaterialInstance* MI_StorageOutline;
};
