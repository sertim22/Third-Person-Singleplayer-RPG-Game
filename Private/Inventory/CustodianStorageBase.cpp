// Copyright Sertim all rights reserved


#include "Inventory/CustodianStorageBase.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/MeshComponent.h"
#include "Inventory/CustodianInventoryComponent.h"


ACustodianStorageBase::ACustodianStorageBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StorageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StorageMesh"));
	SetRootComponent(StorageMesh);
	StorageMesh->SetSimulatePhysics(false);

	RotationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RotationComponent"));
	RotationComponent->SetupAttachment(GetRootComponent());
	RotationComponent->bHiddenInGame = true;

	MovableStorageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MovableStorageMesh"));
	MovableStorageMesh->SetupAttachment(RotationComponent);
	MovableStorageMesh->SetSimulatePhysics(false);

	StorageOverlapSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("StorageOverlapSphereCollision"));
	StorageOverlapSphereCollision->SetupAttachment(GetRootComponent());

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(GetRootComponent());
	InteractWidget->SetVisibility(false);
}

void ACustodianStorageBase::Interact_Implementation(UCustodianInventoryComponent* InInventoryComponent)
{
	StorageInteract();
}

void ACustodianStorageBase::RemoveItemFromStorage(int32 ItemIndex)
{
	FItemMaster ItemMaster;
	ItemMaster.Quantity = 0;
	ItemsInStorage[ItemIndex] = ItemMaster;
}

void ACustodianStorageBase::BeginPlay()
{
	Super::BeginPlay();

	StorageOverlapSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACustodianStorageBase::OnSphereOverlap);
	StorageOverlapSphereCollision->OnComponentEndOverlap.AddDynamic(this, &ACustodianStorageBase::OnSphereEndOverlap);
}

void ACustodianStorageBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromResult, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (OtherActor->ActorHasTag("Player"))
	{
		StorageMesh->SetOverlayMaterial(MI_StorageOutline);
		MovableStorageMesh->SetOverlayMaterial(MI_StorageOutline);
		InteractWidget->SetVisibility(true);
	}
}

void ACustodianStorageBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority()) return;

	if (OtherActor->ActorHasTag("Player"))
	{
		StorageMesh->SetOverlayMaterial(nullptr);
		MovableStorageMesh->SetOverlayMaterial(nullptr);
		InteractWidget->SetVisibility(false);
	}
}


