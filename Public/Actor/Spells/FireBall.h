// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Actor/CustodianProjectile.h"
#include "FireBall.generated.h"

UCLASS()
class CUSTODIAN_API AFireBall : public ACustodianProjectile
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;

protected:

	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromResult, const FHitResult& SweepResult) override;

	virtual void OnHit() override;
};
