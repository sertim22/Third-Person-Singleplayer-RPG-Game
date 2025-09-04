// Copyright Sertim all rights reserved


#include "AssistantFunctionLibrary.h"
#include "Interaction/InventoryInterface.h"
#include "GameFramework/Character.h"
#include "Player/CustodianPlayerController.h"
#include "Inventory/CustodianInventoryComponent.h"
#include "Inventory/WeaponInfo.h"
#include "GameplayEffect.h"


void UAssistantFunctionLibrary::LoadAndEquipSoftGearMesh(ACharacter* InCharacter, EEquipmentSlot InEquipmentSlot,
	TSoftObjectPtr<USkeletalMesh> InSoftSkeletalMesh, TSoftObjectPtr<UStaticMesh> InSoftStaticMesh)
{
	switch (InEquipmentSlot)
	{
	case EEquipmentSlot::EES_Head: 
		IInventoryInterface::Execute_EquipHeadGear(InCharacter, InSoftSkeletalMesh.LoadSynchronous());
		break;
	case EEquipmentSlot::EES_Chest:
		IInventoryInterface::Execute_EquipChestGear(InCharacter, InSoftSkeletalMesh.LoadSynchronous());
		break;
	case EEquipmentSlot::EES_Arms:
		IInventoryInterface::Execute_EquipArmsGear(InCharacter, InSoftSkeletalMesh.LoadSynchronous());
		break;
	case EEquipmentSlot::EES_Legs:
		IInventoryInterface::Execute_EquipLegsGear(InCharacter, InSoftSkeletalMesh.LoadSynchronous());
		break;
	case EEquipmentSlot::EES_RightHandMeleeWeapon:
		SetIsWieldingMeleeWeapon(InCharacter, true);
		IInventoryInterface::Execute_EquipRightHandMeleeWeapon(InCharacter, InSoftStaticMesh.LoadSynchronous());
		break;
	case EEquipmentSlot::EES_LeftHandMeleeWeapon:
		IInventoryInterface::Execute_EquipLeftHandMeleeWeapon(InCharacter, InSoftStaticMesh.LoadSynchronous());
		break;
	case EEquipmentSlot::EES_RangedWeapon:
		SetIsWieldingRangedWeapon(InCharacter, true);
		IInventoryInterface::Execute_EquipRangedWeaponMesh(InCharacter, InSoftSkeletalMesh.LoadSynchronous());
		break;
	case EEquipmentSlot::EES_Consumables:
		SetIsWieldingConsumable(InCharacter, true);
		break;
	default:
		break;
	}
}

void UAssistantFunctionLibrary::UnloadAndUnequipSoftGearMesh(ACharacter* InCharacter, EEquipmentSlot InEquipmentSlot)
{
	switch (InEquipmentSlot)
	{
	case EEquipmentSlot::EES_Head:
		IInventoryInterface::Execute_EquipHeadGear(InCharacter, nullptr);
		break;
	case EEquipmentSlot::EES_Chest:
		IInventoryInterface::Execute_EquipChestGear(InCharacter, nullptr);
		break;
	case EEquipmentSlot::EES_Arms:
		IInventoryInterface::Execute_EquipArmsGear(InCharacter, nullptr);
		break;
	case EEquipmentSlot::EES_Legs:
		IInventoryInterface::Execute_EquipLegsGear(InCharacter, nullptr);
		break;
	case EEquipmentSlot::EES_RightHandMeleeWeapon:
		SetIsWieldingMeleeWeapon(InCharacter, false);
		IInventoryInterface::Execute_EquipRightHandMeleeWeapon(InCharacter, nullptr);
		break;
	case EEquipmentSlot::EES_LeftHandMeleeWeapon:
		IInventoryInterface::Execute_EquipLeftHandMeleeWeapon(InCharacter, nullptr);
		break;
	case EEquipmentSlot::EES_RangedWeapon:
		SetIsWieldingRangedWeapon(InCharacter, false);
		IInventoryInterface::Execute_EquipRangedWeaponMesh(InCharacter, nullptr);
		break;
	case EEquipmentSlot::EES_Consumables:
		SetIsWieldingConsumable(InCharacter, false);
		break;
	default:
		break;
	}
}

void UAssistantFunctionLibrary::SetIsWieldingMeleeWeapon(ACharacter* InCharacter, bool IsWieldingMeleeWeapon)
{
	if (InCharacter)
	{
		GetCustodianPC(InCharacter)->bIsWieldingMeleeWeapon = IsWieldingMeleeWeapon;
	}
}

void UAssistantFunctionLibrary::SetIsWieldingRangedWeapon(ACharacter* InCharacter, bool IsWieldingRangedWeapon)
{
	if (InCharacter)
	{
		GetCustodianPC(InCharacter)->bIsWieldingRangedWeapon = IsWieldingRangedWeapon;
	}
}

void UAssistantFunctionLibrary::SetIsWieldingConsumable(ACharacter* InCharacter, bool IsWieldingConsumable)
{
	if (InCharacter)
	{
		GetCustodianPC(InCharacter)->bIsWieldingConsumable = IsWieldingConsumable;
	}
}

void UAssistantFunctionLibrary::SetEquipmentTags(UCustodianInventoryComponent* InInventoryComponent, EEquipmentSlot InItemEquipmentSlot, FGameplayTag InItemTag)
{
	if (InInventoryComponent)
	{
		switch (InItemEquipmentSlot)
		{
		case EEquipmentSlot::EES_Head:
			InInventoryComponent->HeadGearTag = InItemTag;
			break;
		case EEquipmentSlot::EES_Chest:
			InInventoryComponent->ChestGearTag = InItemTag;
			break;
		case EEquipmentSlot::EES_Arms:
			InInventoryComponent->ArmsGearTag = InItemTag;
			break;
		case EEquipmentSlot::EES_Legs:
			InInventoryComponent->LegsGearTag = InItemTag;
			break;
		case EEquipmentSlot::EES_RightHandMeleeWeapon:
			InInventoryComponent->RightHandWeaponTag = InItemTag;
			break;
		case EEquipmentSlot::EES_LeftHandMeleeWeapon:
			InInventoryComponent->LeftHandWeaponTag = InItemTag;
			break;
		case EEquipmentSlot::EES_RangedWeapon:
			InInventoryComponent->RangedWeaponTag = InItemTag;
			break;
		case EEquipmentSlot::EES_Consumables:
			InInventoryComponent->ConsumablesTag = InItemTag;
			break;
		default:
			break;
		}
	}
}

void UAssistantFunctionLibrary::AddToArmorClassByTag(UCustodianInventoryComponent* InInventoryComponent, EEquipmentSlot InItemEquipmentSlot, FGameplayTag InItemTag)
{
	if (InInventoryComponent)
	{
		// Make sure to clean ArmorClass of the Item before adding
		SubtractFromArmorClass(InInventoryComponent, InItemEquipmentSlot);

		AActor* IC_Owner = InInventoryComponent->GetOwner();
		switch (InItemEquipmentSlot)
		{
		case EEquipmentSlot::EES_Head:
			IInventoryInterface::Execute_SetArmorClass(IC_Owner, IInventoryInterface::Execute_GetArmorClass(IC_Owner) + GetArmorClassByTag(InItemTag, InInventoryComponent));
			InInventoryComponent->HeadArmorClass = GetArmorClassByTag(InItemTag, InInventoryComponent);
			InInventoryComponent->UpdateArmorClassDelegate.Broadcast();
			break;
		case EEquipmentSlot::EES_Chest:
			IInventoryInterface::Execute_SetArmorClass(IC_Owner, IInventoryInterface::Execute_GetArmorClass(IC_Owner) + GetArmorClassByTag(InItemTag, InInventoryComponent));
			InInventoryComponent->ChestArmorClass = GetArmorClassByTag(InItemTag, InInventoryComponent);
			InInventoryComponent->UpdateArmorClassDelegate.Broadcast();
			break;
		case EEquipmentSlot::EES_Arms:
			IInventoryInterface::Execute_SetArmorClass(IC_Owner, IInventoryInterface::Execute_GetArmorClass(IC_Owner) + GetArmorClassByTag(InItemTag, InInventoryComponent));
			InInventoryComponent->ArmsArmorClass = GetArmorClassByTag(InItemTag, InInventoryComponent);
			InInventoryComponent->UpdateArmorClassDelegate.Broadcast();
			break;
		case EEquipmentSlot::EES_Legs:
			IInventoryInterface::Execute_SetArmorClass(IC_Owner, IInventoryInterface::Execute_GetArmorClass(IC_Owner) + GetArmorClassByTag(InItemTag, InInventoryComponent));
			InInventoryComponent->LegsArmorClass = GetArmorClassByTag(InItemTag, InInventoryComponent);
			InInventoryComponent->UpdateArmorClassDelegate.Broadcast();
			break;
		default:
			break;
		}
	}
}

void UAssistantFunctionLibrary::SubtractFromArmorClass(UCustodianInventoryComponent* InInventoryComponent, EEquipmentSlot InItemEquipmentSlot)
{
	if (InInventoryComponent)
	{
		AActor* IC_Owner = InInventoryComponent->GetOwner();
		switch (InItemEquipmentSlot)
		{
		case EEquipmentSlot::EES_Head:
			IInventoryInterface::Execute_SetArmorClass(IC_Owner, IInventoryInterface::Execute_GetArmorClass(IC_Owner) - InInventoryComponent->HeadArmorClass);
			InInventoryComponent->HeadArmorClass = 0.f;
			InInventoryComponent->UpdateArmorClassDelegate.Broadcast();
			break;
		case EEquipmentSlot::EES_Chest:
			IInventoryInterface::Execute_SetArmorClass(IC_Owner, IInventoryInterface::Execute_GetArmorClass(IC_Owner) - InInventoryComponent->ChestArmorClass);
			InInventoryComponent->ChestArmorClass = 0.f;
			InInventoryComponent->UpdateArmorClassDelegate.Broadcast();
			break;
		case EEquipmentSlot::EES_Arms:
			IInventoryInterface::Execute_SetArmorClass(IC_Owner, IInventoryInterface::Execute_GetArmorClass(IC_Owner) - InInventoryComponent->ArmsArmorClass);
			InInventoryComponent->ArmsArmorClass = 0.f;
			InInventoryComponent->UpdateArmorClassDelegate.Broadcast();
			break;
		case EEquipmentSlot::EES_Legs:
			IInventoryInterface::Execute_SetArmorClass(IC_Owner, IInventoryInterface::Execute_GetArmorClass(IC_Owner) - InInventoryComponent->LegsArmorClass);
			InInventoryComponent->LegsArmorClass = 0.f;
			InInventoryComponent->UpdateArmorClassDelegate.Broadcast();
			break;
		default:
			break;
		}
	}
}

bool UAssistantFunctionLibrary::GetIsWieldingMeleeWeapon(ACharacter* InCharacter)
{
	if (InCharacter)
	{
		return GetCustodianPC(InCharacter)->bIsWieldingMeleeWeapon;
	}
	return false;
}

bool UAssistantFunctionLibrary::GetIsWieldingRangedWeapon(ACharacter* InCharacter)
{
	if (InCharacter)
	{
		return GetCustodianPC(InCharacter)->bIsWieldingRangedWeapon;
	}
	return false;
}

bool UAssistantFunctionLibrary::GetIsWieldingConsumable(ACharacter* InCharacter)
{
	if (InCharacter)
	{
		return GetCustodianPC(InCharacter)->bIsWieldingConsumable;
	}
	return false;
}

float UAssistantFunctionLibrary::GetWeaponPowerByTag(FGameplayTag ItemTag, UCustodianInventoryComponent* InInventoryComponent)
{
	if (InInventoryComponent && InInventoryComponent->WeaponInfo)
	{
		return InInventoryComponent->WeaponInfo->GetWeaponPowerByTag(ItemTag);
	}
	return 0.f;
}

float UAssistantFunctionLibrary::GetArmorClassByTag(FGameplayTag ItemTag, UCustodianInventoryComponent* InInventoryComponent)
{
	if (InInventoryComponent && InInventoryComponent->ArmorInfo)
	{
		return InInventoryComponent->ArmorInfo->GetArmorClassByTag(ItemTag);
	}
	return 0.f;
}

TSubclassOf<UGameplayEffect> UAssistantFunctionLibrary::GetConsumableEffectClassByTag(FGameplayTag ItemTag, UCustodianInventoryComponent* InInventoryComponent)
{
	if (InInventoryComponent && InInventoryComponent->ConsumablesInfo)
	{
		return InInventoryComponent->ConsumablesInfo->GetConsumableEffectByTag(ItemTag);
	}
	return nullptr;
}

EEffectType UAssistantFunctionLibrary::GetConsumableEffectTypeByTag(FGameplayTag ItemTag, UCustodianInventoryComponent* InInventoryComponent)
{
	if (InInventoryComponent && InInventoryComponent->ConsumablesInfo)
	{
		return InInventoryComponent->ConsumablesInfo->GetConsumableEffectTypeByTag(ItemTag);
	}
	return EEffectType::EET_None;
}

float UAssistantFunctionLibrary::GetGameplayEffectModifierValue(TSubclassOf<UGameplayEffect> InEffectClass, int32 Level)
{
	if (!InEffectClass) return 0.f;

	float OutValue = 0.f;
	UGameplayEffect* Effect = Cast<UGameplayEffect>(InEffectClass->GetDefaultObject());
	for (const FGameplayModifierInfo& Modifier : Effect->Modifiers)
	{
		float Magnitude = 0.f;
		Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, Magnitude);
		OutValue += Magnitude;
	}
	return OutValue;
}

float UAssistantFunctionLibrary::GetGameplayEffectFrequencyValue(TSubclassOf<UGameplayEffect> InEffectClass, int32 Level)
{
	if (!InEffectClass) return 0.f;

	const UGameplayEffect* Effect = Cast<UGameplayEffect>(InEffectClass->GetDefaultObject());
	if (!Effect) return 0.f;

	float Period = Effect->Period.GetValueAtLevel(Level);
	if (Period < 0.f) return 0.f;
	
	float Duration = 0.f;
	EGameplayEffectMagnitudeCalculation EGEMC_Type = Effect->DurationMagnitude.GetMagnitudeCalculationType();
	switch (EGEMC_Type)
	{
	case EGameplayEffectMagnitudeCalculation::ScalableFloat:
		Effect->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, Duration);
	}
	return Duration / Period;
}

ACustodianPlayerController* UAssistantFunctionLibrary::GetCustodianPC(ACharacter* InCharacter)
{
	ACustodianPlayerController* CustodianPC = Cast<ACustodianPlayerController>(InCharacter->GetController());
	return CustodianPC;
}
