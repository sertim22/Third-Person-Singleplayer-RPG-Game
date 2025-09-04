// Copyright Sertim all rights reserved


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/CustodianAbilitySystemComponent.h"
#include "AbilitySystem/CustodianAttributeSet.h"
#include "Player/CustodianPlayerState.h"
#include "CustodianGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/CustodianPlayerController.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetCustodianAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetCustodianAS()->GetMaxHealth());
	OnStaminaChanged.Broadcast(GetCustodianAS()->GetStamina());
	OnMaxStaminaChanged.Broadcast(GetCustodianAS()->GetMaxStamina());
	OnManaChanged.Broadcast(GetCustodianAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetCustodianAS()->GetMaxMana());

	OnCharacterStanceChangedDelegate.Broadcast(UCustodianAbilitySystemLibrary::GetCharacterStance(GetCustodianPC()->GetCustodianCharacter()));
	OnInventoryStateDelegate.Broadcast(GetCustodianPC()->bIsInventoryOpened);
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetCustodianAS()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetCustodianAS()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetCustodianAS()->GetStaminaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnStaminaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetCustodianAS()->GetMaxStaminaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxStaminaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetCustodianAS()->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetCustodianAS()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	if (GetCustodianASC())
	{
		GetCustodianASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		/*if (GetCustodianASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetCustodianASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}*/

		GetCustodianASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));

					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot) const
{
	const FCustodianGameplayTags& GameplayTags = FCustodianGameplayTags::Get();

	FCustodianAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PrevSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty Info if Previous Slot is a valid slot. Only if equipping an already-equipped spell.
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// Fill in the new slot.
	FCustodianAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}
