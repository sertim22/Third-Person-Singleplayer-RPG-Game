// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "CustodianAbilitySystemLibrary.generated.h"

class UAttributeMenuWidgetController;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class USpellMenuWidgetController;
class UAbilityInfo;
class UCustodianSaveGame;
struct FWidgetControllerParams;
struct FDamageEffectParams;
class ULootTiers;
class ACustodianHUD;
class USplineComponent;
class ACustodianPlayerController;

// This function library class grants access to different variable on many classes.
// Use it to get Widget Controllers, Ability System Default components, damage parameteres, Gameplay Mechanics variables,
// Character Defaults. It also contains Effect Context Setters and Getters
UCLASS()
class CUSTODIAN_API UCustodianAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*
	* Gameplay Abilities
	*/

	// Get GameplayAbilitySpecHandle by passing in AbilityTag and AbilitySystemComponent
	UFUNCTION(BlueprintPure, Category = "Custodian Gameplay Ability", meta = (ReturnDisplayName = "OutSpec"))
	static FGameplayAbilitySpecHandle GetAbilitySpecHandleByTag(FGameplayTag InAbilityTag, UAbilitySystemComponent* InASC);

	/*
	* Gameplay Abilities end
	*/


	/*
	* Widget Controller
	*/

	// Creates Widget Controller parameteres to get access to the Player Controller, Player State,
	// Attribute Set and Ability System Component of the Player Character
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"), meta = (ReturnDisplayName = "Success"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ACustodianHUD*& OutCustodianHUD);

	// Checks if Widget Controller Parameteres were created successfully and returns the Overlay Widget Controller
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"), meta = (ReturnDisplayName = "WidgetController"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	// Checks if Widget Controller Parameteres were created successfully and returns the AttributeMenu Widget Controller
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"), meta = (ReturnDisplayName = "WidgetController"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	// Checks if Widget Controller Parameteres were created successfully and returns the SpellMenu Widget Controller
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"), meta = (ReturnDisplayName = "WidgetController"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	/*
	* Widget Controller end
	*/


	/*
	* Ability System Class Defaults
	*/
	
	// Call this function to initialize Primary, Secondary and Vital attributes after starting a new game
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	// Call this function to initialize Primary, Secondary and Vital attributes when loading a save game file
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, UCustodianSaveGame* SaveGame);

	// This function grants Startup Abilities. In other words the abilities that the character has when starting a game
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	// Get Character Class Information such as XP reward, primary and secondary attributes
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|CharacterClassDefaults", meta = (ReturnDisplayName = "CharacterClassInfo"))
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	// Get Ability Information such as Icon image, input and ability tags and etc.
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|CharacterClassDefaults", meta = (ReturnDisplayName = "AbilityInfo"))
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|CharacterClassDefaults", meta = (DefaultToSelf = "WorldContextObject"), meta = (ReturnDisplayName = "LootTiers"))
	static ULootTiers* GetLootTiers(const UObject* WorldContextObject);

	/*
	* Ability System Class Defaults end
	*/


	/*
	* Effect Context Getters
	*/

	// Check if the damage was blocked (if it was cut in half)
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "Success"))
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	// Determine whether apply Debuff Gameplay Effect or not
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "Success"))
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);

	// Get Debuff Damage amount
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "OutDamage"))
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	// Get Debuff Gameplay Effect duration
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "OutDuration"))
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	// Get Debuff Gameplay Effect frequency
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "OutFrequency"))
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	// Get Damage Type gameplay tag
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "DamageType"))
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	// Get Death Impulse direction vector
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "Vector"))
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	// Get Knockback Force Direction vector
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "Vector"))
	static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);

	// Determine if there was a successful critical hit (if the damage should be doubled + add CriticalHitDamage bonus)
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "Success"))
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	// Determine if damage caused by ability is radial (this type of damage has radial falloff)
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "IsRadialDamage"))
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	// Get RadialDamageInnerRadius. The inner radius always has the same amount of damage applied to the target
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "Radius"))
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	// Get RadialDamageOuterRadius. The outer radius has a radial falloff which means that
	// damage applied to the target shrinks closer to the border of the outer radius
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "Radius"))
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	// Get the center of the radial damage sphere
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayEffects", meta = (ReturnDisplayName = "Vector"))
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);

	/*
	* Effect Context Getters end
	*/


	/*
	* Effect Context Setters
	*/

	// Sets the CustodianAbilityTypes variable bIsBlockedHit to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	// Sets the CustodianAbilityTypes variable bIsCriticalHit to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	// Sets the CustodianAbilityTypes variable bIsSuccessfulDebuff to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff);

	// Sets the CustodianAbilityTypes variable DebuffDamage to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDamage);

	// Sets the CustodianAbilityTypes variable DebuffDuration to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDuration);

	// Sets the CustodianAbilityTypes variable DebuffFrequency to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InFrequency);

	// Sets the CustodianAbilityTypes variable DamageType to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	// Sets the CustodianAbilityTypes variable DeathImpulse to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse);

	// Sets the CustodianAbilityTypes variable KnockbackForce to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce);

	// Sets the CustodianAbilityTypes variable bIsRadialDamage to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage);

	// Sets the CustodianAbilityTypes variable RadialDamageInnerRadius to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius);

	// Sets the CustodianAbilityTypes variable RadialDamageOuterRadius to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius);

	// Sets the CustodianAbilityTypes variable RadialDamageOrigin to the value passed in
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin);

	/*
	* Effect Context Setters end
	*/


	/*
	* Gameplay Mechanics
	*/

	// Get the closest target for the AI character during the fight
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayMechanics")
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);

	// Function applies damage effect
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|DamageEffect", meta = (ReturnDisplayName = "DamageEffect"))
	static FGameplayEffectContextHandle ApplyDamageEffect(FDamageEffectParams DamageEffectParams);

	// Places number of Rotators passed in at the even locations between each other
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayMechanics", meta = (ReturnDisplayName = "Rotators"))
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);

	// Rotates number of Vectors passed in to the even directions
	UFUNCTION(BlueprintPure, Category = "CustodianAbilitySystemLibrary|GameplayMechanics", meta = (ReturnDisplayName = "Vectors"))
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);

	// Gets the experience reward for the character class and character level passed in
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayMechanics", meta = (ReturnDisplayName = "Location"))
	static FVector GetCameraPointLocation(ACharacter* InCharacter, float MaxTrace);

	// Get the direction from which the Player starts swinging Melee Weapon
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayMechanics", meta = (ReturnDisplayName = "CameraDirection"))
	static ECameraDirection GetPlayerCameraDirection(AController* InController);

	// The function must be called whenever the stamina is being Changed.
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|GameplayMechanics")
	static void RegenerateCharacterStamina(AActor* Owner);

	/*
	* Gameplay Mechanics end
	*/


	/*
	* Character Defaults
	*/

	// Gets Character Stance variable
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults", meta = (ReturnDisplayName = "CharacterStance"))
	static ECharacterStance GetCharacterStance(ACharacter* InCharacter);
	
	// Sets Character Stance to Default Stance
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults")
	static void SetCharacterStanceToDefault(ACharacter* InCharacter);

	// Sets Character Stance to Magic Stance
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults")
	static void SetCharacterStanceToMagic(ACharacter* InCharacter);

	// Sets Character Stance to Melee Stance
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults")
	static void SetCharacterStanceToMelee(ACharacter* InCharacter);

	// Sets Character Stance to Ranged Stance
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults")
	static void SetCharacterStanceToRanged(ACharacter* InCharacter);

	// Checks if the Character Stance equals Default Stance
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults", meta = (ReturnDisplayName = "Success"))
	static bool IsInDefaultStance(ACharacter* InCharacter);

	// Checks if the Character Stance equals Magic Stance
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults", meta = (ReturnDisplayName = "Success"))
	static bool IsInMagicStance(ACharacter* InCharacter);

	// Checks if the Character Stance equals Melee Stance
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults", meta = (ReturnDisplayName = "Success"))
	static bool IsInMeleeStance(ACharacter* InCharacter);

	// Checks if the Character Stance equals Ranged Stance
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults", meta = (ReturnDisplayName = "Success"))
	static bool IsInRangedStance(ACharacter* InCharacter);

	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults")
	static void ExitTeleportStance(ACharacter* InCharacter);

	// Sets inventory state to "opened"
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults|Inventory System")
	static void OpenInventory(ACharacter* InCharacter);

	// Sets inventory state to "closed"
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults|Inventory System")
	static void CloseInventory(ACharacter* InCharacter);
	
	// Gets current Inventory State to see if its opened or not
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|Character Defaults|Inventory System", meta = (ReturnDisplayName = "IsOpened"))
	static bool GetInventoryState(ACharacter* InCharacter);

	// Checks if the Player Character is Aiming
	static bool IsAiming(ACharacter* InCharacter);
	// Checks if the Player Character is Firing
	static bool IsFiring(ACharacter* InCharacter);
	// Checks if the Character is Blocking
	static bool IsBlocking(ACharacter* InCharacter);
	// Checks if the Character is PreAttacking (Is trying to swing the Melee Weapon)
	static bool IsPreAttacking(ACharacter* InCharacter);
	// Checks if the Character is Attacking
	static bool IsAttacking(ACharacter* InCharacter);
	// Checks if the Character is Encumbered (Carrying too much weight)
	static bool IsEncumbered(ACharacter* InCharacter);
	// Broadcasts OnCharacterStanceChangedDelegate to the Character Essentials widget blueprint
	static void BroadcastCharacterStance(ECharacterStance InCharacterStance, const UObject* WorldContextObject);
	// Broadcasts OnCharacterStanceChangedDelegate to the Overlay widget blueprint
	static void BroadcastInventoryState(const UObject* WorldContextObject, bool bInventoryState);

	/*
	* Character Defaults end
	*/


	/*
	* Damage Effect Params
	*/

	// Defines if the damage should be radial. If bIsRadial equals true then specify Inner and Outer radius as well as its Origin
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|DamageEffectParameters")
	static void SetIsRadialDamageEffectParam(UPARAM(ref) FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin);

	// Defines in what direction should the target be thrown back and with what magnitude
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|DamageEffectParameters")
	static void SetKnockbackDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude = 0.f);

	// Defines in what direction should the target be thrown back and with what magnitude when it dies
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|DamageEffectParameters")
	static void SetDeathImpulseDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude = 0.f);

	// Sets Damage Effect Parameters for the Target of the caused damage
	UFUNCTION(BlueprintCallable, Category = "CustodianAbilitySystemLibrary|DamageEffectParameters")
	static void SetTargetEffectParamsASC(UPARAM(ref) FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC);

	/*
	* Damage Effect Params end
	*/

private:

	// Private function of the UCustodianAbilitySystemLibrary class that helps to get CustodianPlayerController class
	static ACustodianPlayerController* GetCustodianPC(ACharacter* InCharacter);
};
