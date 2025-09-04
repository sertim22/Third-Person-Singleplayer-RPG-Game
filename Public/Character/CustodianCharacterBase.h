// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/InventoryInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CustodianCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;
class UNiagaraSystem;
class UParticleSystem;
class UDebuffNiagaraComponent;
class UPassiveNiagaraComponent;
class UBoxComponent;

// This enum contains different character stances that can change during the game and affect animations, abilities and ai behavior
UENUM(BlueprintType)
enum class ECharacterStance : uint8
{
	ECS_DefaultStance,
	ECS_MeleeStance,
	ECS_RangedStance,
	ECS_MagicStance
};

// This enum contains different attack directions that are used by AI to attack and block in the right direction
UENUM(BlueprintType)
enum class EAttackDirection : uint8
{
	EAD_None UMETA(DisplayName = "None"),
	EAD_TopDown UMETA(DisplayName = "Top Down"),
	EAD_BottomUp UMETA(DisplayName = "Bottom Up"),
	EAD_Left UMETA(DisplayName = "Left"),
	EAD_Right UMETA(DisplayName = "Right")
};

// This enum helps to play different animations for the Ranged Weapon Mesh
UENUM(BlueprintType)
enum class ERangedWeaponState : uint8
{
	ERWS_Idle UMETA(DisplayName = "Idle"),
	ERWS_Aiming UMETA(DisplayName = "Aiming"),
	ERWS_Draw UMETA(DisplayName = "Draw"),
	ERWS_Loose UMETA(DisplayName = "Loose")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlockEndedSignature);

// A base class for all characters in game. Add functions and variables to this class if you want them to be shared
// between player character class (CustodianCharacter) and AI character class (CustodianEnemy). This class implements
// following interfaces: IAbilitySystemInterface, ICombatInterface, IInventoryInterface
UCLASS()
class CUSTODIAN_API ACustodianCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface, public IInventoryInterface
{
	GENERATED_BODY()

public:
	
	ACustodianCharacterBase();

	virtual void Tick(float DeltaTime) override;
	// Function is resposible for taking damage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Gets Character's Ability System Component
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// Gets Character's Attribute Set
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/*Combat Interface*/
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual UParticleSystem* GetBloodEffect_Implementation() override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual void SetIsBeingShocked_Implementation(bool bInShock) override;
	virtual bool IsBeingShocked_Implementation() const override;
	virtual FOnDamageSignature& GetOnDamageDelegate() override;
	virtual bool ReserveAttackToken_Implementation(int32 Amount) override;
	virtual void ReturnAttackToken_Implementation(int32 Amount) override;
	// Vital Attributes Getters
	virtual float GetCharacterHealth_Implementation() override;
	virtual float GetCharacterMaxHealth_Implementation() override;
	virtual float GetCharacterStamina_Implementation() override;
	virtual float GetCharacterMaxStamina_Implementation() override;
	virtual float GetCharacterMana_Implementation() override;
	virtual float GetCharacterMaxMana_Implementation() override;
	// end Vital Attributes Getters
	// Character Essentials
	/*Setters*/
	virtual void SetShouldApplyRangedDamage_Implementation(bool InApply) override { bShouldApplyRangedDamage = InApply; }
	virtual void SetRangedDamageMultiplier_Implementation(float InMultiplier) override { RangedDamageMultiplier = InMultiplier; }
	virtual void SetIsBlocking_Implementation(bool ShouldBlock) override { bIsBlocking = ShouldBlock; }
	virtual void SetIsPreAttacking_Implementation(bool IsPreAttacking) override { bIsPreAttacking = IsPreAttacking; }
	virtual void SetIsAttacking_Implementation(bool IsAttacking) override { bIsAttacking = IsAttacking; }
	virtual void SetIsIgnoringDebuffs_Implementation(bool ShouldIgnore) override { bIsIgnoringDebuffs = ShouldIgnore; }
	virtual void SetIsSprinting_Implementation(bool IsSprinting) override { bIsSprinting = IsSprinting; }
	/*Getters*/
	virtual bool GetShouldApplyRangedDamage_Implementation() override { return bShouldApplyRangedDamage; }
	virtual float GetRangedDamageMultiplier_Implementation() override { return RangedDamageMultiplier; }
	virtual bool GetIsBlocking_Implementation() override { return bIsBlocking; }
	virtual bool GetIsPreAttacking_Implementation() override { return bIsPreAttacking; }
	virtual bool GetIsAttacking_Implementation() override { return bIsAttacking; }
	virtual bool GetIsIgnoringDebuffs_Implementation() override { return bIsIgnoringDebuffs; }
	virtual TSubclassOf<UGameplayEffect> GetStaminaRegenEffectClass_Implementation() override { return StaminaRegenerationEffectClass; }
	virtual bool GetIsSprinting_Implementation() override { return bIsSprinting; }
	// end Character Essentials
	virtual void CheckCharacterStamina_Implementation() override { CheckStamina(); }
	virtual void CallOnBlockEnded_Implementation() override { OnBlockEndedDelegate.Broadcast(); }
	/*end Combat Interface*/

	/*Inventory Interface*/
	virtual int32 GetCharacterGold_Implementation() override;
	virtual void UpdateGoldAmount_Implementation(int32 InGoldAmount) override;
	virtual void EquipHeadGear_Implementation(UObject* InHeadGearMesh = nullptr) override;
	virtual void EquipChestGear_Implementation(UObject* InChestGearMesh = nullptr) override;
	virtual void EquipArmsGear_Implementation(UObject* InArmsGearMesh = nullptr) override;
	virtual void EquipLegsGear_Implementation(UObject* InLegsGearMesh = nullptr) override;
	virtual void EquipRightHandMeleeWeapon_Implementation(UObject* InRightHandMeleeWeaponMesh = nullptr) override;
	virtual void EquipLeftHandMeleeWeapon_Implementation(UObject* InLeftHandMeleeWeaponMesh = nullptr) override;
	virtual void EquipRangedWeaponMesh_Implementation(UObject* InRangedWeaponMesh = nullptr) override;
	virtual void SetMeleeWeaponTag_Implementation(FGameplayTag InMeleeWeaponTag) override { MeleeWeaponTag = InMeleeWeaponTag; }
	virtual FGameplayTag GetMeleeWeaponTag_Implementation() override { return MeleeWeaponTag; }
	virtual void SetRangedWeaponTag_Implementation(FGameplayTag InRangedWeaponTag) override { RangedWeaponTag = InRangedWeaponTag; }
	virtual FGameplayTag GetRangedWeaponTag_Implementation() override { return RangedWeaponTag; }
	virtual void SetArmorClass_Implementation(float InArmorClass) override { ArmorClass = InArmorClass; }
	virtual float GetArmorClass_Implementation() override { return ArmorClass; }
	virtual void AddToArmorClass_Implementation(float InArmorClass) override { ArmorClass += InArmorClass; }
	virtual void SetIsUsingConsumable_Implementation(bool bIsUsing) override { bIsUsingConsumable = bIsUsing; }
	virtual bool GetIsUsingConsumable_Implementation() override { return bIsUsingConsumable; }
	/*end Inventory Interface*/

	// Sets character class to the value passed in
	void SetCharacterClass(ECharacterClass InClass) { CharacterClass = InClass; }

	/*
	* Delegates
	*/

	// Must be broadcasted in child classes of CustodianCharacterBase in "InitAbilityActorInfo" function
	// in order to assign ASC for the character. Otherwise ASC will be null pointer
	FOnASCRegistered OnASCRegistered;
	// Must be broadcasted in "Die" function
	FOnDeathSignature OnDeathDelegate;
	// Must be broadcasted in "TakeDamage" function
	FOnDamageSignature OnDamageDelegate;

	// Bind event this delegate in Block abilities to set WeaponCollisionBox no longer overlapping
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Block Ability")
	FOnBlockEndedSignature OnBlockEndedDelegate;

	/*
	* Delegates end
	*/


	/*
	* Character Stance
	*/

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character Stance")
	ECharacterStance CharacterStance = ECharacterStance::ECS_DefaultStance;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character Stance")
	EAttackDirection AttackDirection = EAttackDirection::EAD_None;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character Stance")
	ERangedWeaponState RangedWeaponState = ERangedWeaponState::ERWS_Idle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Stance|Gameplay Mechanics")
	TSubclassOf<UGameplayEffect> StaminaRegenerationEffectClass;

	// Timer for keeping track of Stamina recovery.
	// It is used to invalidate timer in "RegenerateCharacterStamina" function.
	FTimerHandle StaminaCheckTimerHandle;

	void CheckStamina();

	UPROPERTY(BlueprintReadOnly, Category = "Character Stance|Gameplay Mechanics")
	bool bIsStunned = false;

	UPROPERTY(BlueprintReadOnly, Category = "Character Stance|Gameplay Mechanics")
	bool bIsBurned = false;

	UPROPERTY(BlueprintReadOnly, Category = "Character Stance|Gameplay Mechanics")
	bool bIsBeingShocked = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character Stance|Gameplay Mechanics")
	bool bIsBlocking = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character Stance|Gameplay Mechanics")
	bool bIsAttacking = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character Stance|Gameplay Mechanics")
	bool bIsPreAttacking = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character Stance|Gameplay Mechanics")
	bool bIsUsingConsumable = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character Stance|Gameplay Mechanics")
	bool bIsIgnoringDebuffs = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character Stance|Gameplay Mechanics")
	bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stance|Gameplay Mechanics")
	int32 AttackTokenCount = 1;

	/*
	* Character Stance end
	*/


	/*
	* Inventory System
	*/

	UPROPERTY(BlueprintReadWrite, Category = "Inventory System|Weight")
	bool bIsEncumbered = false;

	// This variable is responsible for causing Ranged Damage to the Target with such weapons as Bows, Crossbows, Ballistas and etc.
	// Call "ICombatInterface" events in "Missile-Type" actors (ArrowMissile and etc.) to set it
	UPROPERTY(BlueprintReadWrite, Category = "Inventory System|Combat")
	bool bShouldApplyRangedDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Gold")
	int32 Gold = 0;

	// Increases Character's chance to get BlockedHit when being damaged
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Armor Class")
	float ArmorClass = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory System|Armor&Equipment")
	TObjectPtr<USkeletalMeshComponent> HeadGearMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory System|Armor&Equipment")
	TObjectPtr<USkeletalMeshComponent> ChestGearMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory System|Armor&Equipment")
	TObjectPtr<USkeletalMeshComponent> ArmsGearMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory System|Armor&Equipment")
	TObjectPtr<USkeletalMeshComponent> LegsGearMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory System|Armor&Equipment")
	TObjectPtr<UStaticMeshComponent> RightHandMeleeWeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory System|Armor&Equipment")
	TObjectPtr<UStaticMeshComponent> LeftHandMeleeWeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory System|Armor&Equipment")
	TObjectPtr<UBoxComponent> RightHandWeaponCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory System|Armor&Equipment")
	TObjectPtr<USkeletalMeshComponent> RangedWeaponMesh;

	// Currently used MeleeWeapon. Call "SetMeleeWeaponTag" in melee abilities
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory System|Armor&Equipment")
	FGameplayTag MeleeWeaponTag;

	// Currently used RangedWeapon. Call "SetRangedWeaponTag" in Missile Actors (ArrowMissile and etc.)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory System|Armor&Equipment")
	FGameplayTag RangedWeaponTag;

	/*
	* Inventory System end
	*/


	// Variable responsible for Applying Damage caused by Ranged Weapon. Call "SetRangedDamageMultiplier" in your ability classes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = "0.001", ClampMax = "1.0"))
	float RangedDamageMultiplier = FMath::Clamp(RangedDamageMultiplier, 0.001, 1.0);

protected:

	virtual void BeginPlay() override;
	// Override this function in child classes
	virtual void InitAbilityActorInfo();
	
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	/* Socket Names */
	UPROPERTY(EditAnywhere, Category = "Combat|Sockets")
	FName LeftHandSocketName = "LeftHandSocket";

	UPROPERTY(EditAnywhere, Category = "Combat|Sockets")
	FName RightHandSocketName = "RightHandSocket";

	UPROPERTY(EditAnywhere, Category = "Combat|Sockets")
	FName ArrowSocketName = "ArrowSocket";

	UPROPERTY(EditAnywhere, Category = "Combat|Sockets")
	FName ProjectileSocketName = "ProjectileSocket";

	UPROPERTY(EditAnywhere, Category = "Combat|Sockets")
	FName ScabbardSocketName = "ScabbardSocket";

	UPROPERTY(EditAnywhere, Category = "Combat|Sockets")
	FName SpineSocketName = "SpineSocket";

	UPROPERTY(EditAnywhere, Category = "Combat|Sockets")
	FName ShieldSpineSocketName = "ShieldSpineSocket";
	/* Socket names end */

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
	float BaseWalkSpeed = 600.f;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	// Apllies gameplay effect to the owner 
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const;
	// Applies Attributes Gameplay Effects to initialize them
	virtual void InitializeDefaultAttributes() const;
	// Grants character startup and passive abilities
	void AddCharacterAbilities();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects|Combat")
	UParticleSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects|Combat")
	USoundBase* DeathSound;

	UPROPERTY(VisibleAnywhere, Category = "Effects|Combat|Debuff")
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	UPROPERTY(VisibleAnywhere, Category = "Effects|Combat|Debuff")
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;

	int32 MinionCount = 0;

private:

	UPROPERTY(EditAnywhere, Category = "AbilitySystem|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "AbilitySystem|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat|Hit React")
	TObjectPtr<UAnimMontage> HitReactMontage;

	// This is a Root Scene Component for all Niagara Effects used by the Character
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	TObjectPtr<USceneComponent> EffectAttachComponent;

	UPROPERTY(VisibleAnywhere, Category = "Effects|Passive Spells")
	TObjectPtr<UPassiveNiagaraComponent> PassiveSpellNiagaraComponent_01;

	UPROPERTY(VisibleAnywhere, Category = "Effects|Passive Spells")
	TObjectPtr<UPassiveNiagaraComponent> PassiveSpellNiagaraComponent_02;
};
