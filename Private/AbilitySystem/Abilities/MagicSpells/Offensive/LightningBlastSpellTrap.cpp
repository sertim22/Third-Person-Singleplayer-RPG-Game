// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/MagicSpells/Offensive/LightningBlastSpellTrap.h"

FString ULightningBlastSpellTrap::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>Lightning Blast Spell Trap</>\n\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		"<Default>Cast this spell to set a Lightning Blast trap, "
		"which causes Lightning Damage </> (<Lightning>%d</>) "
		"<Default>and puts the enemy into a</> <Lightning>Stunned</> <Default>State</>"),

		// Values
		Level,
		ManaCost,
		Cooldown,
		ScaledDamage);
}

FString ULightningBlastSpellTrap::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>Lightning Blast Spell Trap (Next Lvl)</>\n\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		"<Default>Cast this spell to set a Lightning Blast trap, "
		"which causes Lightning Damage </> (<Lightning>%d</>) "
		"<Default>and puts the enemy into a</> <Lightning>Stunned</> <Default>State</>"),

		// Values
		Level,
		ManaCost,
		Cooldown,
		ScaledDamage);
}
