// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint/Checkpoint.h"
#include "MapEntrance.generated.h"

UCLASS()
class CUSTODIAN_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()
	
public:

	AMapEntrance(const FObjectInitializer& ObjectInitializer);

	/* Save Interface */
	virtual void LoadActor_Implementation() override;
	/* end Save Interface */

	/* Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	/* end Highlight Interface */

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;

protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromResult, const FHitResult& SweepResult) override;
};
