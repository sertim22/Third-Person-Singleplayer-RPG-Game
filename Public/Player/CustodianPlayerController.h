// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Interaction/PlayerInterface.h"
#include "CustodianPlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCustodianAbilitySystemComponent;
class UCustodianInputConfig;
class UDamageTextComponent;
class UNiagaraSystem;
class IHighlightInterface;
class ACharacter;
class UOverlayWidgetController;

// Use this enum to enter desired Character Stance in (Enter"Desired"Stance) functions when Switching between stances
enum class EDesiredStance : uint8
{
	EDS_DefaultStance,
	EDS_RangedStance,
	EDS_MeleeStance,
	EDS_MagicStance
};

UENUM(BlueprintType)
// Enum containing 4 different camera directions helping to play correct Attack and Block Animations and Animation Montages
enum class ECameraDirection : uint8
{
	None,
	TopDown,
	BottomUp,
	Left,
	Right
};

// This class is used by CustodianCharacter (PlayerCharacter). Create a blueprint version
// in the editor and assign InputActions and MappingContexts there. Also assign
// Player Controller class in GameMode blueprint
UCLASS()
class CUSTODIAN_API ACustodianPlayerController : public APlayerController, public IPlayerInterface
{
	GENERATED_BODY()
	
public:

	ACustodianPlayerController();
	// Returns the owner of this class (CustodianCharacter). However return type is just "ACharacter". Keep that in mind!
	ACharacter* GetCustodianCharacter();

	// Shows the amount of damage caused by player
	UFUNCTION(Server, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Stance")
	bool bIsFiring = false;

	bool bIsAiming = false;

	bool bIsBlocking = false;

	bool bIsInventoryOpened = false;

	bool bIsWieldingMeleeWeapon = false;

	bool bIsWieldingRangedWeapon = false;

	bool bIsWieldingConsumable = false;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	ECameraDirection PlayerCameraDirection = ECameraDirection::None;

	/*Player Interface*/
	virtual void PlayerEnterDefaultStance_Implementation() override { EnterDefaultStance(); }
	/*Player Interface end*/
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

private:

	/*
	* Mapping Contexts
	*/

	UPROPERTY(EditAnywhere, Category = "Input|Mapping Contexts")
	TObjectPtr<UInputMappingContext> CustodianMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input|Mapping Contexts")
	TObjectPtr<UInputMappingContext> RangedMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input|Mapping Contexts")
	TObjectPtr<UInputMappingContext> AimRangedMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input|Mapping Contexts")
	TObjectPtr<UInputMappingContext>MagicMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input|Mapping Contexts")
	TObjectPtr<UInputMappingContext> AimMagicMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input|Mapping Contexts")
	TObjectPtr<UInputMappingContext>MeleeMappingContext;

	/*
	* Mapping Contexts end
	*/


	/*
	* Input Actions
	*/

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> DrawRangedAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> FireRangedAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> OffensiveSpell_01_Action;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> OffensiveSpell_02_Action;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> RegenerativeSpell_01_Action;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> RegenerativeSpell_02_Action;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> SwingMeleeWeaponAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> FlyAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> SwitchToDefaultStanceAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> SwitchToRangedStanceAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> SwitchToMagicStanceAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> SwitchToMeleeStanceAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> BlockAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> OpenInventoryAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> UseConsumableItemAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> QuickSaveAction;

	UPROPERTY(EditAnywhere, Category = "Input|Input Actions")
	TObjectPtr<UInputAction> DeleteSaveAction;

	/*
	* Input Actions end
	*/


	// Assign this Input Config in order to be able to activate Gameplay Abilities with the correct pressed input button.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UCustodianInputConfig> InputConfig;

	// Reference to the Player's Ability System Component
	UPROPERTY()
	TObjectPtr<UCustodianAbilitySystemComponent> CustodianAbilitySystemComponent;
	// Returns casted CustodianAbilitySystemComponent
	UCustodianAbilitySystemComponent* GetASC();

	/*Input Functions*/
	void Move(const FInputActionValue& Value);
	void Fly(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJump();
	void Sprint();
	void StopSprint();
	void Aim();
	void StopAim();
	void DrawRanged();
	void FireRanged();
	void FireSpell();
	void FireSpellEnd();
	void StartSwingMeleeWeapon();
	void EndSwingMeleeWeapon();
	void EnterDefaultStance();
	void EnterMagicStance();
	void EnterRangedStance();
	void EnterMeleeStance();
	void RegenerativeSpell();
	void StartBlock();
	void EndBlock();
	void OpenInventory();
	void Interact();
	void UseConsumableItem();
	void QuickSave();
	void DeleteSave();
	/*Input Functions end*/

	// Bind this Ability Action to the InputConfig in "SetupInputComponent" function
	void AbilityInputTagPressed(FGameplayTag InputTag);
	// Bind this Ability Action to the InputConfig in "SetupInputComponent" function
	void AbilityInputTagReleased(FGameplayTag InputTag);
	// Bind this Ability Action to the InputConfig in "SetupInputComponent" function
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	bool bEquippedRangedWeapon = false;
	bool bEquippedMeleeWeapon = false;
	bool CanFire = false;
	// Checks if the player has applicable CharacterStance for Aiming
	bool CanAim();

	// Checks current character's stance to add a mapping context you need.
	void AddApplicableMappingContext(UInputMappingContext* InMappingContext);
	// Checks the Desired Stance passed in to handle equipping and unequipping different items before entering it.
	// Use "EDesiredStance" enum to pass in the stance type
	bool CanEnterDesiredStance(EDesiredStance InDesiredStance);
	// Calculates the direction in which the player moves the camera
	void CalculatePlayerCameraDirection();
};
