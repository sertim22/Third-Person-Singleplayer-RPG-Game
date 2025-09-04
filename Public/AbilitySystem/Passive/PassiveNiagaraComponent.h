// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "PassiveNiagaraComponent.generated.h"

class UCustodianAbilitySystemComponent;

USTRUCT(BlueprintType)
struct CUSTODIAN_API FPassiveNiagaraEffects : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag EffectTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UNiagaraSystem> EffectSystem = nullptr;
};

UCLASS()
class CUSTODIAN_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
	
public:

	UPassiveNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveEffectTag;

	FPassiveNiagaraEffects PassiveEffectsStruct;

protected:

	virtual void BeginPlay() override;
	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);
	void ActivateIfEquipped(UCustodianAbilitySystemComponent* CustodianASC);

	void LoadEffectFromDataTable();
};
