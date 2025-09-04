// Copyright Sertim all rights reserved


#include "UI/WidgetController/CustodianWidgetController.h"
#include "Player/CustodianPlayerController.h"
#include "Player/CustodianPlayerState.h"
#include "AbilitySystem/CustodianAbilitySystemComponent.h"
#include "AbilitySystem/CustodianAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"


void UCustodianWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UCustodianWidgetController::BroadcastInitialValues()
{

}

void UCustodianWidgetController::BindCallbacksToDependencies()
{
	
}

void UCustodianWidgetController::BroadcastAbilityInfo()
{
	if (!GetCustodianASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FCustodianAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(CustodianAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = CustodianAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = CustodianAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	GetCustodianASC()->ForEachAbility(BroadcastDelegate);
}

ACustodianPlayerController* UCustodianWidgetController::GetCustodianPC()
{
	if (CustodianPlayerController == nullptr)
	{
		CustodianPlayerController = Cast<ACustodianPlayerController>(PlayerController);
	}
	return CustodianPlayerController;
}

ACustodianPlayerState* UCustodianWidgetController::GetCustodianPS()
{
	if (CustodianPlayerState == nullptr)
	{
		CustodianPlayerState = Cast<ACustodianPlayerState>(PlayerState);
	}
	return CustodianPlayerState;
}

UCustodianAbilitySystemComponent* UCustodianWidgetController::GetCustodianASC()
{
	if (CustodianAbilitySystemComponent == nullptr)
	{
		CustodianAbilitySystemComponent = Cast<UCustodianAbilitySystemComponent>(AbilitySystemComponent);
	}
	return CustodianAbilitySystemComponent;
}

UCustodianAttributeSet* UCustodianWidgetController::GetCustodianAS()
{
	if (CustodianAttributeSet == nullptr)
	{
		CustodianAttributeSet = Cast<UCustodianAttributeSet>(AttributeSet);
	}
	return CustodianAttributeSet;
}
