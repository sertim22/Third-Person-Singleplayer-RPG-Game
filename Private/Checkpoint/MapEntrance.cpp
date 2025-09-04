// Copyright Sertim all rights reserved


#include "Checkpoint/MapEntrance.h"
#include "Interaction/PlayerInterface.h"
#include "Game/CustodianGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Sphere->SetupAttachment(MoveToComponent);
}

void AMapEntrance::LoadActor_Implementation()
{
	// do nothing when loading a map entrance
}

void AMapEntrance::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromResult, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;

		if (ACustodianGameModeBase* CustodianGM = Cast<ACustodianGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			CustodianGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}

		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);

		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
