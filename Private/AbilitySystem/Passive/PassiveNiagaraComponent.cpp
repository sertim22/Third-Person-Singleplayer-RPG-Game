// Copyright Sertim all rights reserved


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "AbilitySystem/CustodianAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/CombatInterface.h"
#include "CustodianGameplayTags.h"


UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UCustodianAbilitySystemComponent* CustodianASC = Cast<UCustodianAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		CustodianASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
		ActivateIfEquipped(CustodianASC);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
			{
				if (UCustodianAbilitySystemComponent* CustodianASC = Cast<UCustodianAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
				{
					CustodianASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
					ActivateIfEquipped(CustodianASC);
				}
			});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (AbilityTag.MatchesTagExact(PassiveEffectTag))
	{
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}

void UPassiveNiagaraComponent::ActivateIfEquipped(UCustodianAbilitySystemComponent* CustodianASC)
{
	const bool bStartupAbilitiesGiven = CustodianASC->bStartupAbilitiesGiven;
	if (bStartupAbilitiesGiven)
	{
		if (CustodianASC->GetStatusFromAbilityTag(PassiveEffectTag) == FCustodianGameplayTags::Get().Abilities_Status_Equipped)
		{
			Activate();
		}
	}
}
