// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MagicSpells/Offensive/SpellTrap.h"
#include "LightningBlastSpellTrap.generated.h"

// This class is derived from the SpellTrap class and grants the ability to cast Lightning Traps that cause
// Stunn Debuff and damage Enemies overlapping with it
UCLASS()
class CUSTODIAN_API ULightningBlastSpellTrap : public USpellTrap
{
	GENERATED_BODY()
	
public:

	// Spell Description
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	// Spell Description end
};
