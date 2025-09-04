// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "CustodianAbilitySystemGlobals.generated.h"

UCLASS()
class CUSTODIAN_API UCustodianAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
