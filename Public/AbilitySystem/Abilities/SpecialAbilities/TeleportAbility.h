// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianGameplayAbility.h"
#include "TeleportAbility.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class CUSTODIAN_API UTeleportAbility : public UCustodianGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Teleport")
	void StartTeleport();

	UFUNCTION(BlueprintCallable, Category = "Teleport")
	void EndTeleport();

	bool CanActivateAbility();

	UPROPERTY(EditAnywhere, Category = "Teleport Properties")
	float TeleportAcceptanceRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = "Teleport Properties")
	float TeleportAccelerationSpeed = 1250.f;

	UPROPERTY(EditAnywhere, Category = "Teleport Properties")
	float MaxTeleportSpeed = 1250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport Properties")
	TObjectPtr<UNiagaraSystem> TeleportNiagaraSystem;

	UNiagaraComponent* TeleportNSRef = nullptr;
};
