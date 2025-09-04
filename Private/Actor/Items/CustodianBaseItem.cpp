// Copyright Sertim all rights reserved


#include "Actor/Items/CustodianBaseItem.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/MeshComponent.h"
#include "Inventory/CustodianInventoryComponent.h"


ACustodianBaseItem::ACustodianBaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);
	ItemMesh->SetSimulatePhysics(true);

	ItemOverlapSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ItemOverlapSphereCollision"));
	ItemOverlapSphereCollision->SetupAttachment(GetRootComponent());

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(GetRootComponent());
	InteractWidget->SetVisibility(false);
}

void ACustodianBaseItem::Interact_Implementation(UCustodianInventoryComponent* InInventoryComponent)
{
	bool AddedItemToInventory = InInventoryComponent->AddItemToInventory(ItemMasterData);
	if (AddedItemToInventory)
	{
		if (InInventoryComponent->LoadInventoryDelegate.IsBound())
		{
			InInventoryComponent->LoadInventoryDelegate.Broadcast();
			InInventoryComponent->UpdateWeightDelegate.Broadcast();
			Destroy();
		}
	}
	else
	{
		IInventoryInterface::Execute_AddInventoryIsFullWidget(this);
	}
}

void ACustodianBaseItem::BeginPlay()
{
	Super::BeginPlay();

	ItemOverlapSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACustodianBaseItem::OnSphereOverlap);
	ItemOverlapSphereCollision->OnComponentEndOverlap.AddDynamic(this, &ACustodianBaseItem::OnSphereEndOverlap);
}

void ACustodianBaseItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromResult, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (OtherActor->ActorHasTag("Player"))
	{
		ItemMesh->SetOverlayMaterial(MI_ItemOutline);
		InteractWidget->SetVisibility(true);
	}
}

void ACustodianBaseItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority()) return;

	if (OtherActor->ActorHasTag("Player"))
	{
		ItemMesh->SetOverlayMaterial(nullptr);
		InteractWidget->SetVisibility(false);
	}
}

