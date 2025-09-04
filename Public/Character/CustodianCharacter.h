// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Character/CustodianCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "CustodianCharacter.generated.h"

class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;

// Player Character class for the Custodian project. Create a player character blueprint based on this class.
// It implements player and combat interface functions
UCLASS()
class CUSTODIAN_API ACustodianCharacter : public ACustodianCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
	
public:

	ACustodianCharacter();

	/* Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void SaveProgress_Implementation(const FName& CheckpointTag) override;
	virtual USpringArmComponent* GetPlayerSpringArmComponent_Implementation() override { return CameraBoom; }
	/* end Player Interface */

	/** Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual void HealCharacter_Implementation(float HealPercentage) override;
	virtual void Die(const FVector& DeathImpulse) override;
	/** end Combat Interface */

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float DeathTime = 5.f;

	// Timer that must be set when the character dies
	FTimerHandle DeathTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects|Level Up")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Level Up")
	TObjectPtr<USoundBase> LevelUpSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Level Up")
	float LevelUpEffectDurationTime = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

protected:

	virtual void BeginPlay() override;

	// This function must be called in BeginPlay, to load the progress from the save file
	void LoadProgress();

private:

	// This function must be called in BeginPlay, to initialize ability system for the player
	virtual void InitAbilityActorInfo() override;
};
