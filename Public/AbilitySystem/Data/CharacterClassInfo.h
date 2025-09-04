// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

// Enum contaning the different character classes with unic abilties and AI behavior
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Mage,
	Ranger,
	Warrior
};

// Struct that holds the information about primary attributes, startup abilities and experience reward for killing the character of this class 
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Class Defualts")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Class Defualts")
	FScalableFloat XPReward = FScalableFloat();
};

// Create UCharacterClassInfo data asset to assert abilities, attributes and XP reward for all the character classes in your game
UCLASS()
class CUSTODIAN_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// A TMap that converts character class into the FCharacterClassDefaultInfo struct
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defualts")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;


	/*
	* Common Class Defualts
	*/

	// This Gameplay Effect class is used to initialize Primary Attributes from the save file when loading or starting a new game
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defualts")
	TSubclassOf<UGameplayEffect> PrimaryAttributes_SetByCaller;

	// This attributes Gameplay Effect class only applies for the AI characters. If you want to choose the Secondary Attributes
	// GE for the Player Character you must do so in its Blueprint Class
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defualts")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	// This Gameplay Effect class is used to get information about Secondary attributes from the save data when loading a save file or
	// when starting a new game
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defualts")
	TSubclassOf<UGameplayEffect> SecondaryAttributes_Infinite;

	// This Gameplay Effect class is used to get the Vital Attributes values such as Health, Stamina and Mana.
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defualts")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	// This array contains GameplayAbilities that are shared between all AI characters in the game. 
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defualts")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	// The Curve Table that is used calculate Damage Parameters such as: Effective Armor, Armor Penetration and Critical Hit Resistance
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defualts|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficents;

	/*
	* Common Class Defualts end
	*/

	// Function returns FCharacterClassDefaultInfo struct for the CharacterClass passed in 
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
