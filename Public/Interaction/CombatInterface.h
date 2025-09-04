// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
class UParticleSystem;
class UAnimMontage;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature, float /*DamageAmount*/);

UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

// This interface class is inherited by all character classes that have combat system.
// It contains vital attribute getters, AI behavior mechanics, and character essentials getters and events
class CUSTODIAN_API ICombatInterface
{
	GENERATED_BODY()

public:

	/*
	* AI Behavior
	*/

	// Gets a team number which is used by AI to define whether he sees an enemy or not
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|AI Behavior", meta = (ReturnDisplayName = "TeamNumber"))
	int32 GetTeamNumber();
	
	// Call this event to start blocking hits
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|AI Behavior")
	void BlockStarted();

	// The amount of tokens define how many enemies can attack one target. This event is called to take
	// some of the free tokens so the enemies don't attack all at once
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|AI Behavior", meta = (ReturnDisplayName = "ReservedToken"))
	bool ReserveAttackToken(int32 Amount);

	// This event is called to free reserved tokens after attack is finished
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|AI Behavior")
	void ReturnAttackToken(int32 Amount);

	// This event makes AI move towards the target if the target is trying to dodge the hit
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|AI Behavior")
	void GetCloserToAttackTarget(AActor* AttackTarget);

	/*
	* AI Behavior end
	*/


	/*
	* Inventory System
	*/

	// Call this event to equip ranged weapon
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Inventory System")
	void EquipRangedWeapon(bool bEquippedRangedWeapon);

	// Call this event to equip melee weapon
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Inventory System")
	void EquipMeleeWeapon(bool bEquippedMeleeWeapon);

	// Call this event to unequip ranged weapon
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Inventory System")
	void UnequipRangedWeapon(bool bEquippedRangedWeapon);

	// Call this event to unequip melee weapon
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Inventory System")
	void UnequipMeleeWeapon(bool bEquippedMeleeWeapon);

	/*
	* Inventory System end
	*/


	/*
	* Vital Attribute Getters
	*/

	// This function returns current character's level
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Vital Attribute Getters", meta = (ReturnDisplayName = "Level"))
	int32 GetCharacterLevel();

	// This function returns current character's Health
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Vital Attribute Getters", meta = (ReturnDisplayName = "CurrentHealth"))
	float GetCharacterHealth();

	// This function returns current character's MaxHealth
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Vital Attribute Getters", meta = (ReturnDisplayName = "MaxHealth"))
	float GetCharacterMaxHealth();

	// This function returns current character's Stamina
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Vital Attribute Getters", meta = (ReturnDisplayName = "CurrentStamina"))
	float GetCharacterStamina();

	// This function returns current character's MaxStamina
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Vital Attribute Getters", meta = (ReturnDisplayName = "MaxStamina"))
	float GetCharacterMaxStamina();

	// This function returns current character's Mana
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Vital Attribute Getters", meta = (ReturnDisplayName = "CurrentMana"))
	float GetCharacterMana();

	// This function returns current character's MaxMana
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Vital Attribute Getters", meta = (ReturnDisplayName = "MaxMana"))
	float GetCharacterMaxMana();

	/*
	* Vital Attribute Getters end
	*/


	/*
	* Character Essentials
	*/

	/*Getters*/

	// Implement this function in the custodian character base class to get the OnASCRegistered delegate.
	// Broadcast it in the child classes in InitAbilityActorInfo function
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;
	// Use this function to get the OnDeath Delegate which must have binding in base character class when the character dies 
	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;
	// The function must be implemented on the base character class and used in the ExecCalcDamage class to help calculate the damage
	virtual FOnDamageSignature& GetOnDamageDelegate() = 0;

	// Call this event to check if the character is dead
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ReturnDisplayName = "IsDead"), Category = "Combat Interface|Character Essentials")
	bool IsDead() const;

	// Returns the avatar of the actor that calls this event
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ReturnDisplayName = "AvatarActor"), Category = "Combat Interface|Character Essentials")
	AActor* GetAvatar();

	// Returns character class
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayName = "CharacterClass"))
	ECharacterClass GetCharacterClass();

	// Checks if the target is being shocked
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayName = "IsBeingShocked"))
	bool IsBeingShocked() const;

	// Returns bShouldApplyRangedDamage variable on CustodianCharacterBase
	UFUNCTION(BlueprintNativeEvent, Category = "Combat Interface|Character Essentials")
	bool GetShouldApplyRangedDamage();

	// Returns "RangedDamageMultiplier" variable on the CustodianCharacterBase
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayName = "OutMultiplier"))
	float GetRangedDamageMultiplier();

	// Returns "bIsBlocking" variable from the CustodianCharacterBase class
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayName = "IsBlocking"))
	bool GetIsBlocking();

	// Returns "bIsPreAttacking" varialbe from the CustodianCharacterBase class
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayName = "IsPreAttacking"))
	bool GetIsPreAttacking();

	// Returns "bIsAttacking" varialbe from the CustodianCharacterBase class
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayName = "IsAttacking"))
	bool GetIsAttacking();

	// Call this event in player controller class to prevent from spamming arrows and implement it in "BP_CustodianCharacter"
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayname = "CanDraw"))
	bool CanDrawRanged();

	// Returns "IsDrawing" variable from the "AC_RangedWeaponMechanics" Blueprint
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayname = "IsDrawing"))
	bool GetIsDrawing();

	// Returns "IsWaitingToDraw" variable from the "AC_RangedWeaponMechanics" Blueprint
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayname = "IsWaiting"))
	bool GetIsWaitingToDraw();

	// Returns "IsFiring" variable from the "AC_RangedWeaponMechanics" Blueprint
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayname = "IsFiring"))
	bool GetIsFiring();

	// Returns "IsIgnoringDebuffs" variable from the "CustodianCharacterBase" Blueprint
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayname = "IsIgnoring"))
	bool GetIsIgnoringDebuffs();

	// Returns "IsIgnoringDebuffs" variable from the "CustodianCharacterBase" Blueprint
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayname = "OutEffect"))
	TSubclassOf<UGameplayEffect> GetStaminaRegenEffectClass();

	// Returns "IsSprinting" variable from the CustodianCharacterBase class
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials", meta = (ReturnDisplayname = "IsSprinting"))
	bool GetIsSprinting();

	/*Getters end*/


	/*Setters*/

	// Sets character Shock state to the value passed in
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void SetInShockLoop(bool InLoop);

	// Sets IsBeingShocked to value passed in
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void SetIsBeingShocked(bool InShock);

	// Sets bIsRangedDamage variable on CustodianCharacterBase to the value passed in.
	// Don't forget to set the variable back to false after applying damage!
	UFUNCTION(BlueprintNativeEvent, Category = "Combat Interface|Character Essentials")
	void SetShouldApplyRangedDamage(bool InApply);

	// Sets "RangedDamageMultiplier" variable on the CustodianCharacterBase
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void SetRangedDamageMultiplier(float InMultiplier);

	// Sets "bIsBlocking" variable from the CustodianCharacterBase class to the value passed in
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void SetIsBlocking(bool ShouldBlock);

	// Sets "bIsPreAttacking" varialbe from the CustodianCharacterBase class
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void SetIsPreAttacking(bool IsPreAttacking);

	// Sets "bIsAttacking" varialbe from the CustodianCharacterBase class
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void SetIsAttacking(bool IsAttacking);
	
	// Pass the value in to set "bIsAiming" variable
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void SetInAimLoop(bool InLoop);

	// Pass the value in to set "bIsIgnoringDebuffs" variable on the CustodianCharacterBase class
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void SetIsIgnoringDebuffs(bool ShouldIgnore);

	// Pass the value in to set "bIsSprinting" variable on the CustodianCharacterBase class
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void SetIsSprinting(bool IsSprinting);

	// Pass the value in to set "AttackDirection" variable on the CustodianCharacterBase class
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void SetAttackDirection();

	/*Setters end*/


	// Call this event to make character aiming
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void AimBegin();

	// Call this event to stop character aiming
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void AimEnd();

	// Call this event to add HP to the target
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void HealCharacter(float HealPercentage);

	// Call this event in player controller class to prevent from spamming arrows and implement it in "BP_CustodianCharacter"
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void DrawBegin();

	// The function is implemented in the base character class and can be overriden. It handles all the details on character's death
	virtual void Die(const FVector& DeathImpulse) = 0;

	// Call this function to check if should Remove "StaminaRegenerationEffect"
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void CheckCharacterStamina();

	// This function calls the "OnBlockEnded" delegate. You can use it in PlayerControllerClass
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void CallOnBlockEnded();

	// This event calls "SprintEnded" delegate on the "BP_CustodianCharacter"
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface|Character Essentials")
	void CallSprintEnded();

	/*
	* Character Essentials end
	*/


	// Returns location of the socket that has SocketTag
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ReturnDisplayName = "SocketLocation"))
	FVector GetCombatSocketLocation(const FGameplayTag& SocketTag);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Interface")
	void UpdateFacingTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ReturnDisplayName = "Montage"))
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ReturnDisplayName = "Effect"))
	UParticleSystem* GetBloodEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ReturnDisplayName = "Count"))
	int32 GetMinionCount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncrementMinionCount(int32 Amount);
};
