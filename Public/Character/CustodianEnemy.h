// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Character/CustodianCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "CustodianEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class ACustodianAIController;

// Base character class for all AI driven characters
UCLASS()
class CUSTODIAN_API ACustodianEnemy : public ACustodianCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()
	
public:

	ACustodianEnemy();

	virtual void PossessedBy(AController* NewController) override;

	/** Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	/** end Highlight Interface */

	/** Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual float GetCharacterHealth_Implementation() override;
	virtual float GetCharacterMaxHealth_Implementation() override;
	virtual void HealCharacter_Implementation(float HealPercentage) override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	/** end Combat Interface */

	/** Enemy Interface*/
	virtual bool IsCombatTargetAttacking_Implementation() override;
	/** end Enemy Interface*/

	// Called in begin play to initalize Enemy's health attribute
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	// Called in begin play to initalize Enemy's max health attribute
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	// Sets max walking speed to zero when hit react happens 
	void HitReactChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat|Enemy")
	bool bHitReacting = false;

	// For how long should the enemy's mesh stay in game after death
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Enemy")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat|Enemy")
	TObjectPtr<AActor> CombatTarget;

	void SetLevel(int32 InLevel) { Level = InLevel; }

protected:

	virtual void BeginPlay() override;
	// Registers Ability System Component. This function must be called in Begin Play!
	virtual void InitAbilityActorInfo() override;
	// Initialize enemy's attributes. Call this function only on server and in "InitAbilityActorInfo" function
	virtual void InitializeDefaultAttributes() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory System")
	void SpawnLoot();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ACustodianAIController> CustodianAIController;
};
