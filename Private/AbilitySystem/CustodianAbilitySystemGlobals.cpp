// Copyright Sertim all rights reserved


#include "AbilitySystem/CustodianAbilitySystemGlobals.h"
#include "CustodianAbilityTypes.h"

FGameplayEffectContext* UCustodianAbilitySystemGlobals::AllocGameplayEffectContext() const
{
    return new FCustodianGameplayEffectContext();
}
