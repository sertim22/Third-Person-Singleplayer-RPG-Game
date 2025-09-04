// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "CustodianPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /*StatValue*/, bool /* bLevelUp */);

// This is a Player State class that implements IAbilitySystemInterface and must
// have a Blueprint version in the editor and assigned in GameMode blueprint class.
// The class contains Player Level, Experience, Attribute and Spell points variables
UCLASS()
class CUSTODIAN_API ACustodianPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	ACustodianPlayerState();

	// Returns AbilitySystemComponent variable of this class
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// Returns AttributeSet variable of this class
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Assign this data asset that contains information about leveling up
	UPROPERTY(EditDefaultsOnly, Category = "Player Level")
	TObjectPtr<ULevelUpInfo> LevelUpInfo;


	/*
	* Delegates
	*/

	// Call this delegate when gaining XP
	FOnPlayerStatChanged OnXPChangedDelegate;
	// Call this delegate when Leveling Up
	FOnLevelChanged OnLevelChangedDelegate;
	// Call this delegate when Leveling Up
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	// Call this delegate when Leveling Up
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;

	/*
	* Delegates end
	*/


	// Getters
	FORCEINLINE int32 GetPlayerLevel() const { return  Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	// Setters
	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);
	void SetAttributePoints(int32 InPoints);
	void SetSpellPoints(int32 InPoints);
	// Adders
	void AddToXP(int32 AddedXP);
	void AddToLevel(int32 AddedLevel);
	void AddToAttributePoints(int32 AddedAttributePoints);
	void AddToSpellPoints(int32 AddedSpellPoints);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Points")
	int32 AttributePoints = 0;

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	UPROPERTY(VisibleAnywhere, Category = "Player Level")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, Category = "Player Level")
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, Category = "Points")
	int32 SpellPoints = 0;
};
