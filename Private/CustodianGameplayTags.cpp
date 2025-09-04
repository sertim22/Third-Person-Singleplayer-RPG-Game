// Copyright Sertim all rights reserved


#include "CustodianGameplayTags.h"
#include "GameplayTagsManager.h"

FCustodianGameplayTags FCustodianGameplayTags::GameplayTags;

void FCustodianGameplayTags::InitializeNativeGameplayTags()
{
	/*
	* Attributes
	*/
	// Primary Attributes
	GameplayTags.Attributes_Primary_Constitution = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Primary.Constitution", FString("Increases the max amount of health"));
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Primary.Strength", FString("Increases the physical damage"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Primary.Intelligence", FString("Increases the max amount of mana and magic damage"));
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Primary.Dexterity", FString("Increases armor and armor penetration"));
	GameplayTags.Attributes_Primary_Endurance = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Primary.Endurance", FString("Increases the max amount of stamina and recovery speed"));
	// Secondary Attributes
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.MaxHealth", FString("The max amount of character` health"));
	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.MaxStamina", FString("The max amount of character` stamina"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.MaxMana", FString("The max amount of character` mana"));
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.Armor", FString("Reduces damage taken, improves Block Chance"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.ArmorPenetration", FString("Increases the Critical Hit Chance and Ignored Percentage of enemy Armor"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.BlockChance", FString("Improves the chance to cut the incoming damage in half"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.CriticalHitChance", FString("Increases the chance to cause double damage plus Critical Hit Bonus"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.CriticalHitDamage", FString("Increases the amount of Critical Hit Bonus added when a Critical Hit is scored"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.CriticalHitResistance", FString("Improves the chance to avoid a critical hit"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.HealthRegeneration", FString("Improves the speed of health recovering"));
	GameplayTags.Attributes_Secondary_StaminaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.StaminaRegeneration", FString("Improves the speed of stamina recovering"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.ManaRegeneration", FString("Increases the speed of mana recovering"));
	GameplayTags.Attributes_Secondary_MaxWeight = UGameplayTagsManager::Get().AddNativeGameplayTag("Attributes.Secondary.MaxWeight", FString("Increases character's ability to cary more items in their inventory. Depends on Strength attribute"));
	// Meta Attributes
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"), FString("Incoming XP Meta Attribute"));
	// Resistance Attributes
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Resistance to Fire Damage"));
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString("Resistance to Lightning Damage"));
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"), FString("Resistance to Arcane Damage"));
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Resistance to Physical Damage"));
	/*
	* Attributes end
	*/


	/*
	* Equipment
	*/
	// Weapon
	GameplayTags.Equipment_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Weapon"), FString(""));
	GameplayTags.Equipment_Weapon_Sword_WesternSword = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Weapon.Sword.WesternSword"), FString("Simple Western Sword used by Knights"));
	GameplayTags.Equipment_Weapon_Sword_Test = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Weapon.Sword.Test"), FString("Simple Western Sword used by Knights"));
	GameplayTags.Equipment_Weapon_Bow_AspinBow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Weapon.Bow.AspinBow"), FString("Bow made of Aspin. Manually used for hunting"));
	GameplayTags.Equipment_Weapon_Shield_NorseShield = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Weapon.Shield.NorseShield"), FString("Huge shield to protect the raiders from the constant fire"));
	// Armor
	GameplayTags.Equipment_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Armor"), FString(""));
	GameplayTags.Equipment_Armor_Head_IronHelmet = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Armor.Head.IronHelmet"), FString("Helmet made of Iron"));
	GameplayTags.Equipment_Armor_Head_RedHelmet = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Armor.Head.RedHelmet"), FString("Helmet made of Iron"));
	GameplayTags.Equipment_Armor_Chest_IronChestPlate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Armor.Chest.IronChestPlate"), FString("Chest Plate made of Iron"));
	GameplayTags.Equipment_Armor_Arms_IronArmBracers = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Armor.Arms.IronArmBracers"), FString("Arm Bracers made of Iron"));
	GameplayTags.Equipment_Armor_Legs_IronLegBracers = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Armor.Legs.IronLegBracers"), FString("Leg Bracers made of Iron"));
	// Consumables
	GameplayTags.Equipment_Consumable_Potion_SmallHealthPotion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Consumable.Potion.SmallHealthPotion"), FString("Tag for small health potion"));
	GameplayTags.Equipment_Consumable_Potion_SmallManaPotion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Consumable.Potion.SmallManaPotion"), FString("Tag for small mana potion"));
	/*
	* Equipment end
	*/


	/*
	* Input
	*/
	GameplayTags.InputTags_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.LMB", FString("Input Tag for Left Mouse Button"));
	GameplayTags.InputTags_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.RMB", FString("Input Tag for Right Mouse Button"));
	GameplayTags.InputTags_1 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.1", FString("Input Tag for 1 Key"));
	GameplayTags.InputTags_2 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.2", FString("Input Tag for 2 Key"));
	GameplayTags.InputTags_3 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.3", FString("Input Tag for 3 Key"));
	GameplayTags.InputTags_4 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.4", FString("Input Tag for 4 Key"));
	GameplayTags.InputTags_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Passive.1", FString("Input Tag for Passive Ability 1"));
	GameplayTags.InputTags_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Passive.2", FString("Input Tag for Passive Ability 2"));
	// Ability Input Tags
	GameplayTags.InputTags_Fire_Spell = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Fire.Spell", FString("Input Tag for Firing Spell Ability"));
	GameplayTags.InputTags_Fire_Arrow = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Fire.Arrow", FString("Input Tag for Firing Arrow Ability"));
	GameplayTags.InputTags_Draw_Bow = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Draw.Bow", FString("Input Tag for Drawing Bow Ability"));
	GameplayTags.InputTags_Teleport = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Teleport", FString("Input Tag for Teleport Ability"));
	GameplayTags.InputTags_Heal = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Heal", FString("Input Tag for Healing Ability"));
	GameplayTags.InputTags_SwingMeleeWeapon = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.SwingMeleeWeapon", FString("Input Tag for Swing Melee Weapon Ability"));
	GameplayTags.InputTags_StartBlock = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.StartBlock", FString("Input Tag for Block Ability"));
	GameplayTags.InputTags_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Jump", FString("Input Tag for Jumping Ability"));
	GameplayTags.InputTags_Sprint = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Sprint", FString("Input Tag for Sprinting Ability"));
	GameplayTags.InputTags_UseConsumable = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.UseConsumable", FString("Input Tag for Using Consumable Ability"));
	// Spells
	GameplayTags.InputTags_Abilities_Spells_Offensive_01 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Abilities.Spells.Offensive.01", FString("Input tag for Offensive Ability 01"));
	GameplayTags.InputTags_Abilities_Spells_Offensive_02 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Abilities.Spells.Offensive.02", FString("Input tag for Offensive Ability 02"));
	GameplayTags.InputTags_Abilities_Spells_Regenerative_01 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Abilities.Spells.Regenerative.01", FString("Input tag for Regenerative Ability 01"));
	GameplayTags.InputTags_Abilities_Spells_Regenerative_02 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Abilities.Spells.Regenerative.02", FString("Input tag for Regenerative Ability 02"));
	GameplayTags.InputTags_Abilities_Spells_Passive_01 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTags.Abilities.Spells.Passive.01", FString("Input tag for Passive Ability 01"));
	/*
	* Input end
	*/


	/*
	* Debuffs
	*/
	// Debuff Types
	GameplayTags.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Arcane"), FString("Debuff for Arcane damage"));
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn"), FString("Debuff for Fire damage"));
	GameplayTags.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Physical"), FString("Debuff for Physical damage"));
	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Stun"), FString("Debuff for Lightning damage"));
	// Debuff Properties
	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Chance"), FString("Debuff Chance"));
	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Damage"), FString("Debuff damage"));
	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Duration"), FString("Debuff Duration"));
	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Frequency"), FString("Debuff Frequency"));
	/*
	* Debuffs end
	*/


	/*
	* Damage
	*/
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag("Damage", FString("Damage"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag("Damage.Fire", FString("Fire Damage Type"));
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag("Damage.Lightning", FString("Lightning Damage Type"));
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag("Damage.Arcane", FString("Arcane Damage Type"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag("Damage.Physical", FString("Physical Damage Type"));
	/*
	* Damage end
	*/


	/*
	* Abilities
	*/
	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.None", FString("No Ability - nullptr for Ability Tags"));
	// Passive Abilties
	GameplayTags.Abilities_Passive_ListenForEvents = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Passive.ListenForEvents", FString("Listen for Events passive ability"));
	// Ability Statuses
	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Status.Locked", FString("Locked Status"));
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Status.Eligible", FString("Eligible Status"));
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Status.Unlocked", FString("Unlocked Status"));
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Status.Equipped", FString("Equipped Status"));
	// Ability Types
	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Type.Offensive", FString("Offensive Type of Ability"));
	GameplayTags.Abilities_Type_Regenerative = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Type.Regenerative", FString("Regenerative Type of Ability"));
	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Type.Passive", FString("Passive Type of Ability"));
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Type.None", FString("No Ability Type"));
	// Offensive Spells
	GameplayTags.Abilities_Spells_Offensive_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Spells.Offensive.Fire.FireBolt", FString("Ability FireBolt"));
	GameplayTags.Abilities_Spells_Offensive_Fire_BarrageFireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Spells.Offensive.Fire.BarrageFireBolt", FString("Ability Barrage FireBolt"));
	GameplayTags.Abilities_Spells_Offensive_Fire_MeteorBarrage = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Spells.Offensive.Fire.MeteorBarrage", FString("Ability Meteor Barrage"));
	GameplayTags.Abilities_Spells_Offensive_SpellTrap_LightningBlast = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Spells.Offensive.SpellTrap.LightningBlast", FString("Set Lightning Blast spell trap ability"));
	// Regenerative Spells
	GameplayTags.Abilities_Spells_Regenerative_Health_SpawnHealingAura = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Spells.Regenerative.Health.SpawnHealingAura", FString("Spawn Healing Aura ability"));
	GameplayTags.Abilities_Spells_Regenerative_Mana_BloodPact = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Spells.Regenerative.Mana.BloodPact", FString("Sign Blood Pact ability"));
	// Passive Spells
	GameplayTags.Abilities_Spells_Passive_StoneSkin = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Spells.Passive.StoneSkin", FString("Activate Stone Skin ability"));
	GameplayTags.Abilities_Spells_Passive_ColossusMight = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Spells.Passive.ColossusMight", FString("Activate Might of the Colossus ability"));
	// Skirmish Abilities
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Attack", FString("Attack Ability Tag"));
	GameplayTags.Abilities_Attack_ShortRangeAttack = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Attack.ShortRangeAttack", FString("Short range attack ability"));
	GameplayTags.Abilities_Attack_LongRangeAttack = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Attack.LongRangeAttack", FString("Long range attack ability"));
	// Stamina based Abilities
	GameplayTags.Abilities_Stamina_Block = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Stamina.Block", FString("Block Ability"));
	GameplayTags.Abilities_Stamina_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Stamina.Jump", FString("Jump Ability"));
	GameplayTags.Abilities_Stamina_Sprint = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Stamina.Sprint", FString("Sprint Ability"));
	// Special Abilities
	GameplayTags.Abilities_Special_Teleport = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Special.Teleport", FString("Teleport Ability"));
	GameplayTags.Abilities_Special_DrinkPotion = UGameplayTagsManager::Get().AddNativeGameplayTag("Abilities.Special.DrinkPotion", FString("Drink Potion Ability"));
	/*
	* Abilities end
	*/


	/*
	* Sockets
	*/
	GameplayTags.CombatSocket_ArrowSocket = UGameplayTagsManager::Get().AddNativeGameplayTag("CombatSocket.ArrowSocket", FString("ArrowSocket"));
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag("CombatSocket.RightHand", FString("RightHand"));
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag("CombatSocket.LeftHand", FString("LeftHand"));
	GameplayTags.CombatSocket_ProjectileSocket = UGameplayTagsManager::Get().AddNativeGameplayTag("CombatSocket.ProjectileSocket", FString("ProjectileSocket"));
	/*
	* Sockets end
	*/


	/*
	* Player
	*/
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag("Player.Block.InputHeld", FString("Block Input Held callback for Input"));
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag("Player.Block.InputPressed", FString("Block Input Pressed callback for Input"));
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag("Player.Block.InputReleased", FString("Block Input Released callback for Input"));
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag("Player.Block.CursorTrace", FString("Block Tracing under the Cursor"));
	/*
	* Player end
	*/
	

	/*
	* Effects
	*/
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag("Effects.HitReact", FString("Tag granted when Hit Reacting"));
	/*
	* Effects end
	*/


	/*
	* Mappings
	*/
	// Map of Damage Types to Resistances
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	// Map of Damage Types to Debuffs
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	/*
	* Mappings end
	*/
}
