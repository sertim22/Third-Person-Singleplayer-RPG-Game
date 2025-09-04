// Copyright Sertim all rights reserved


#include "Player/CustodianPlayerState.h"
#include "AbilitySystem/CustodianAbilitySystemComponent.h"
#include "AbilitySystem/CustodianAttributeSet.h"


ACustodianPlayerState::ACustodianPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCustodianAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	AttributeSet = CreateDefaultSubobject<UCustodianAttributeSet>(TEXT("AttributeSet"));

}

UAbilitySystemComponent* ACustodianPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACustodianPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ACustodianPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, false);
}

void ACustodianPlayerState::SetAttributePoints(int32 InPoints)
{
	AttributePoints = InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ACustodianPlayerState::SetSpellPoints(int32 InPoints)
{
	SpellPoints = InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ACustodianPlayerState::AddToXP(int32 AddedXP)
{
	XP += AddedXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ACustodianPlayerState::AddToLevel(int32 AddedLevel)
{
	Level += AddedLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void ACustodianPlayerState::AddToAttributePoints(int32 AddedAttributePoints)
{
	AttributePoints += AddedAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ACustodianPlayerState::AddToSpellPoints(int32 AddedSpellPoints)
{
	SpellPoints += AddedSpellPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
