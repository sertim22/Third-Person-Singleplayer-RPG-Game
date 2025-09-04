// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Custodian Gameplay Tags
 *
 * Singleton containing native Gameplay Tags
 */

struct FCustodianGameplayTags
{

public:

	static const FCustodianGameplayTags& Get() { return GameplayTags; }
	// Add new GameplayTags to this function so they are visible in the editor and C++
	static void InitializeNativeGameplayTags();

	// Gameplay Tags


	/*
	* Attributes
	*/
	// Primary Attributes
	FGameplayTag Attributes_Primary_Constitution;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Endurance;
	// Seconadary Attributes
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxStamina;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_StaminaRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxWeight;
	// Meta Attributes
	FGameplayTag Attributes_Meta_IncomingXP;
	// Resistance Attributes
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
	/*
	* Attributes end
	*/ 


	/*
	* Equipment
	*/
	// Weapons
	FGameplayTag Equipment_Weapon;
	FGameplayTag Equipment_Weapon_Sword_WesternSword;
	FGameplayTag Equipment_Weapon_Sword_Test;
	FGameplayTag Equipment_Weapon_Bow_AspinBow;
	FGameplayTag Equipment_Weapon_Shield_NorseShield;
    // Armor
	FGameplayTag Equipment_Armor;
	FGameplayTag Equipment_Armor_Head_IronHelmet;
	FGameplayTag Equipment_Armor_Head_RedHelmet;
	FGameplayTag Equipment_Armor_Chest_IronChestPlate;
	FGameplayTag Equipment_Armor_Arms_IronArmBracers;
	FGameplayTag Equipment_Armor_Legs_IronLegBracers;
	// Consumables
	FGameplayTag Equipment_Consumable_Potion_SmallHealthPotion;
	FGameplayTag Equipment_Consumable_Potion_SmallManaPotion;
	/*
	* Equipment end
	*/


	/*
	* Debuffs
	*/
	// Debuff Types
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;
	// Debuff Properties
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;
	/*
	* Debuffs end
	*/


	/*
	* Damage
	*/
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	/*
	* Damage end
	*/


	/*
	* Input
	*/
	FGameplayTag InputTags_LMB;
	FGameplayTag InputTags_RMB;
	FGameplayTag InputTags_1;
	FGameplayTag InputTags_2;
	FGameplayTag InputTags_3;
	FGameplayTag InputTags_4;
	FGameplayTag InputTags_Passive_1;
	FGameplayTag InputTags_Passive_2;
	// Ability Input tags
	FGameplayTag InputTags_Fire_Spell;
	FGameplayTag InputTags_Fire_Arrow;
	FGameplayTag InputTags_Draw_Bow;
	FGameplayTag InputTags_Teleport;
	FGameplayTag InputTags_Heal;
	FGameplayTag InputTags_SwingMeleeWeapon;
	FGameplayTag InputTags_StartBlock;
	FGameplayTag InputTags_Jump;
	FGameplayTag InputTags_Sprint;
	FGameplayTag InputTags_UseConsumable;
	// Spells
	FGameplayTag InputTags_Abilities_Spells_Offensive_01;
	FGameplayTag InputTags_Abilities_Spells_Offensive_02;
	FGameplayTag InputTags_Abilities_Spells_Regenerative_01;
	FGameplayTag InputTags_Abilities_Spells_Regenerative_02;
	FGameplayTag InputTags_Abilities_Spells_Passive_01;
	/*
	* Input end
	*/


	/*
	* Abilities
	*/
	FGameplayTag Abilities_None;
	// Passive Abilities
	FGameplayTag Abilities_Passive_ListenForEvents;
	// Ability Statuses
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;
	// Ability Types
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Regenerative;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	// Offensive Spells
	FGameplayTag Abilities_Spells_Offensive_Fire_FireBolt;
	FGameplayTag Abilities_Spells_Offensive_Fire_BarrageFireBolt;
	FGameplayTag Abilities_Spells_Offensive_Fire_MeteorBarrage;
	FGameplayTag Abilities_Spells_Offensive_SpellTrap_LightningBlast;
	// Regenerative Spells
	FGameplayTag Abilities_Spells_Regenerative_Health_SpawnHealingAura;
	FGameplayTag Abilities_Spells_Regenerative_Mana_BloodPact;
	// Passive Spells
	FGameplayTag Abilities_Spells_Passive_StoneSkin;
	FGameplayTag Abilities_Spells_Passive_ColossusMight;
	// Skirmish Abilities
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Attack_ShortRangeAttack;
	FGameplayTag Abilities_Attack_LongRangeAttack;
	// Stamina Based Abilities
	FGameplayTag Abilities_Stamina_Block;
	FGameplayTag Abilities_Stamina_Jump;
	FGameplayTag Abilities_Stamina_Sprint;
	// Special Abilities
	FGameplayTag Abilities_Special_Teleport;
	FGameplayTag Abilities_Special_DrinkPotion;
	/*
	* Abilities end
	*/


	/*
	* Sockets
	*/
	FGameplayTag CombatSocket_ArrowSocket;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_ProjectileSocket;
	/*
	* Sockets end
	*/


	/*
	* Player
	*/
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_CursorTrace;
	/*
	* Player end
	*/


	/*
	* Effects
	*/
	FGameplayTag Effects_HitReact;
	/*
	* Effects end
	*/


	/*
	* Mappings
	*/
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
	/*
	* Mappings end
	*/

private:

	static FCustodianGameplayTags GameplayTags;
};
