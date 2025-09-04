// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/MagicSpells/Passive/StoneSkinAbility.h"
#include "Interaction/InventoryInterface.h"
#include "Interaction/CombatInterface.h"
#include "Inventory/CustodianInventoryComponent.h"


FString UStoneSkinAbility::GetDescription(int32 Level)
{
	const int32 ArmorClass = GrantedArmorClass;

	return FString::Printf(TEXT(
		// Title
		"<Title>Stone Skin</>\n\n"

		"<Default>Assign this Passive Spell in the Spell Slot and get, "
		"additional</> <ArmorClass>Armor Class: %d</>. "
		"<Default>This Spell will also grant you Immunity against any Debuffs (Burning, bleeding etc.)"
		"However such weight on your body does NOT let you sprint anymore!</>"),

		// Values
		ArmorClass);
}

FString UStoneSkinAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Passive Spells cannot be upgraded.</>"));
}

void UStoneSkinAbility::EnableStoneSkin()
{
	IInventoryInterface::Execute_AddToArmorClass(GetAvatarActorFromActorInfo(), GrantedArmorClass);
	IInventoryInterface::Execute_GetCustodianInventoryComponent(GetAvatarActorFromActorInfo())->UpdateArmorClassDelegate.Broadcast();
	ICombatInterface::Execute_SetIsIgnoringDebuffs(GetAvatarActorFromActorInfo(), true);
}

void UStoneSkinAbility::DisableStoneSkin()
{
	IInventoryInterface::Execute_AddToArmorClass(GetAvatarActorFromActorInfo(), GrantedArmorClass * (-1));
	IInventoryInterface::Execute_GetCustodianInventoryComponent(GetAvatarActorFromActorInfo())->UpdateArmorClassDelegate.Broadcast();
	ICombatInterface::Execute_SetIsIgnoringDebuffs(GetAvatarActorFromActorInfo(), false);
}
