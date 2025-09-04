// Copyright Sertim all rights reserved


#include "Player/CustodianPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/CustodianInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystem/CustodianAbilitySystemComponent.h"
#include "Custodian/Custodian.h"
#include "CustodianGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"
#include "UI/Widget/DamageTextComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/InventoryInterface.h"
#include "Interaction/PlayerInterface.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "AssistantFunctionLibrary.h"
#include "CustodianGameplayTags.h"


ACustodianPlayerController::ACustodianPlayerController()
{
	
}

ACharacter* ACustodianPlayerController::GetCustodianCharacter()
{
	AActor* Avatar = GetASC()->GetAvatarActor();
	ACharacter* CharacterActor = Cast<ACharacter>(Avatar);
	return CharacterActor;
}

void ACustodianPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void ACustodianPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(CustodianMappingContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(CustodianMappingContext, 0);
	}

	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
}

void ACustodianPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UCustodianInputComponent* CustodianInputComponent = CastChecked<UCustodianInputComponent>(InputComponent);

	/*Locomotion*/
	CustodianInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACustodianPlayerController::Move);
	CustodianInputComponent->BindAction(FlyAction, ETriggerEvent::Triggered, this, &ACustodianPlayerController::Fly);
	CustodianInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustodianPlayerController::Look);
	CustodianInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ACustodianPlayerController::Aim);
	CustodianInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ACustodianPlayerController::StopAim);
	
	/*Spells*/
	CustodianInputComponent->BindAction(OffensiveSpell_01_Action, ETriggerEvent::Started, this, &ACustodianPlayerController::FireSpell);
	CustodianInputComponent->BindAction(OffensiveSpell_01_Action, ETriggerEvent::Completed, this, &ACustodianPlayerController::FireSpellEnd);
	CustodianInputComponent->BindAction(OffensiveSpell_02_Action, ETriggerEvent::Started, this, &ACustodianPlayerController::FireSpell);
	CustodianInputComponent->BindAction(OffensiveSpell_02_Action, ETriggerEvent::Completed, this, &ACustodianPlayerController::FireSpellEnd);
	CustodianInputComponent->BindAction(RegenerativeSpell_01_Action, ETriggerEvent::Completed, this, &ACustodianPlayerController::RegenerativeSpell);
	CustodianInputComponent->BindAction(RegenerativeSpell_02_Action, ETriggerEvent::Completed, this, &ACustodianPlayerController::RegenerativeSpell);
	/*Offensive Abilities*/
	CustodianInputComponent->BindAction(DrawRangedAction, ETriggerEvent::Started, this, &ACustodianPlayerController::DrawRanged);
	CustodianInputComponent->BindAction(FireRangedAction, ETriggerEvent::Completed, this, &ACustodianPlayerController::FireRanged);
	CustodianInputComponent->BindAction(SwingMeleeWeaponAction, ETriggerEvent::Started, this, &ACustodianPlayerController::StartSwingMeleeWeapon);
	CustodianInputComponent->BindAction(SwingMeleeWeaponAction, ETriggerEvent::Completed, this, &ACustodianPlayerController::EndSwingMeleeWeapon);
	/*Switch between Stances*/
	CustodianInputComponent->BindAction(SwitchToDefaultStanceAction, ETriggerEvent::Started, this, &ACustodianPlayerController::EnterDefaultStance);
	CustodianInputComponent->BindAction(SwitchToMagicStanceAction, ETriggerEvent::Started, this, &ACustodianPlayerController::EnterMagicStance);
	CustodianInputComponent->BindAction(SwitchToRangedStanceAction, ETriggerEvent::Started, this, &ACustodianPlayerController::EnterRangedStance);
	CustodianInputComponent->BindAction(SwitchToMeleeStanceAction, ETriggerEvent::Started, this, &ACustodianPlayerController::EnterMeleeStance);
	/*Stamina Abilities*/
	CustodianInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &ACustodianPlayerController::StartBlock);
	CustodianInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ACustodianPlayerController::EndBlock);
	CustodianInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACustodianPlayerController::Jump);
	CustodianInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACustodianPlayerController::StopJump);
	CustodianInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACustodianPlayerController::Sprint);
	CustodianInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACustodianPlayerController::StopSprint);
	CustodianInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &ACustodianPlayerController::StopSprint);
	/*Character interactions*/
	CustodianInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &ACustodianPlayerController::OpenInventory);
	CustodianInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACustodianPlayerController::Interact);
	CustodianInputComponent->BindAction(UseConsumableItemAction, ETriggerEvent::Started, this, &ACustodianPlayerController::UseConsumableItem);
	/*Other input*/
	CustodianInputComponent->BindAction(QuickSaveAction, ETriggerEvent::Started, this, &ACustodianPlayerController::QuickSave);
	CustodianInputComponent->BindAction(DeleteSaveAction, ETriggerEvent::Started, this, &ACustodianPlayerController::DeleteSave);
	// Bind AbilityActions to the Input Config
	CustodianInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ACustodianPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

UCustodianAbilitySystemComponent* ACustodianPlayerController::GetASC()
{
	if (CustodianAbilitySystemComponent == nullptr)
	{
		CustodianAbilitySystemComponent = Cast<UCustodianAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return CustodianAbilitySystemComponent;
}

void ACustodianPlayerController::Move(const FInputActionValue& Value)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FCustodianGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2d InputAxisVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ACustodianPlayerController::Fly(const FInputActionValue& Value)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FCustodianGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2d InputAxisVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();

	const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ACustodianPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(InputAxisVector.X);
		ControlledPawn->AddControllerPitchInput(InputAxisVector.Y);
	}
}

void ACustodianPlayerController::Jump()
{
	
}

void ACustodianPlayerController::StopJump()
{
	
}

void ACustodianPlayerController::Sprint()
{

}

void ACustodianPlayerController::StopSprint()
{
	ICombatInterface::Execute_CallSprintEnded(GetCustodianCharacter());
}

void ACustodianPlayerController::Aim()
{
	if (!bIsAiming && CanAim())
	{
		bIsAiming = true;
		
		ICombatInterface::Execute_SetInAimLoop(GetCustodianCharacter(), bIsAiming);
		ICombatInterface::Execute_AimBegin(GetPawn());

		// Slow down global time dialation if the player is falling and aiming at the same time to create a slow motion effect
		if (GetCustodianCharacter()->GetCharacterMovement()->IsFalling() && bIsAiming)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.6f);
		}
		// Add applicable mapping context based on current character stance
		if (UCustodianAbilitySystemLibrary::IsInRangedStance(GetCustodianCharacter()))
		{
			AddApplicableMappingContext(AimRangedMappingContext);
		}
		if (UCustodianAbilitySystemLibrary::IsInMagicStance(GetCustodianCharacter()))
		{
			AddApplicableMappingContext(AimMagicMappingContext);
		}
	}
}

void ACustodianPlayerController::StopAim()
{
	if (bIsAiming)
	{
		bIsAiming = false;

		ICombatInterface::Execute_SetInAimLoop(GetCustodianCharacter(), bIsAiming);
		ICombatInterface::Execute_AimEnd(GetPawn());

		// Set Global Time dialation back to normal if the player stoped aiming while falling
		if (!bIsAiming)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
		}
		// Add applicable mapping context based on current character stance
		if (UCustodianAbilitySystemLibrary::IsInRangedStance(GetCustodianCharacter()))
		{
			AddApplicableMappingContext(RangedMappingContext);
		}
		if (UCustodianAbilitySystemLibrary::IsInMagicStance(GetCustodianCharacter()))
		{
			AddApplicableMappingContext(MagicMappingContext);
		}
	}
}

void ACustodianPlayerController::DrawRanged()
{
	if (ICombatInterface::Execute_CanDrawRanged(GetCustodianCharacter()) == true 
		|| ICombatInterface::Execute_GetIsWaitingToDraw(GetCustodianCharacter()) == true)
	{
		ICombatInterface::Execute_DrawBegin(GetCustodianCharacter());
	}
}

void ACustodianPlayerController::FireRanged()
{
	
}

void ACustodianPlayerController::FireSpell()
{
	if (UCustodianAbilitySystemLibrary::IsInMagicStance(GetCustodianCharacter()) && !bIsFiring)
	{
		bIsFiring = true;
	}
}

void ACustodianPlayerController::FireSpellEnd()
{
	if (bIsFiring)
	{
		bIsFiring = false;
	}
}

void ACustodianPlayerController::StartSwingMeleeWeapon()
{
	if (UCustodianAbilitySystemLibrary::IsInMeleeStance(GetCustodianCharacter()) && bEquippedMeleeWeapon)
	{
		CalculatePlayerCameraDirection();
		ICombatInterface::Execute_SetIsPreAttacking(GetCustodianCharacter(), true);
		ICombatInterface::Execute_SetAttackDirection(GetCustodianCharacter());
	}
	
}

void ACustodianPlayerController::EndSwingMeleeWeapon()
{
	ICombatInterface::Execute_SetIsPreAttacking(GetCustodianCharacter(), false);
}

void ACustodianPlayerController::EnterDefaultStance()
{
	if (CanEnterDesiredStance(EDesiredStance::EDS_DefaultStance))
	{
		AddApplicableMappingContext(CustodianMappingContext);
		UCustodianAbilitySystemLibrary::SetCharacterStanceToDefault(GetCustodianCharacter());
		UCustodianAbilitySystemLibrary::BroadcastCharacterStance(UCustodianAbilitySystemLibrary::GetCharacterStance(GetCustodianCharacter()), GetCustodianCharacter());
		GetCustodianCharacter()->GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCustodianCharacter()->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
}

void ACustodianPlayerController::EnterMagicStance()
{
	if(CanEnterDesiredStance(EDesiredStance::EDS_MagicStance))
	{
		AddApplicableMappingContext(MagicMappingContext);
		UCustodianAbilitySystemLibrary::SetCharacterStanceToMagic(GetCustodianCharacter());
		UCustodianAbilitySystemLibrary::BroadcastCharacterStance(UCustodianAbilitySystemLibrary::GetCharacterStance(GetCustodianCharacter()), GetCustodianCharacter());
		GetCustodianCharacter()->GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCustodianCharacter()->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
}

void ACustodianPlayerController::EnterRangedStance()
{
	if (CanEnterDesiredStance(EDesiredStance::EDS_RangedStance))
	{
		AddApplicableMappingContext(RangedMappingContext);
		UCustodianAbilitySystemLibrary::SetCharacterStanceToRanged(GetCustodianCharacter());
		UCustodianAbilitySystemLibrary::BroadcastCharacterStance(UCustodianAbilitySystemLibrary::GetCharacterStance(GetCustodianCharacter()), GetCustodianCharacter());
		GetCustodianCharacter()->GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCustodianCharacter()->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
}

void ACustodianPlayerController::EnterMeleeStance()
{
	if (CanEnterDesiredStance(EDesiredStance::EDS_MeleeStance))
	{
		AddApplicableMappingContext(MeleeMappingContext);
		UCustodianAbilitySystemLibrary::SetCharacterStanceToMelee(GetCustodianCharacter());
		UCustodianAbilitySystemLibrary::BroadcastCharacterStance(UCustodianAbilitySystemLibrary::GetCharacterStance(GetCustodianCharacter()), GetCustodianCharacter());
		GetCustodianCharacter()->GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCustodianCharacter()->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void ACustodianPlayerController::RegenerativeSpell()
{
	
}

void ACustodianPlayerController::StartBlock()
{
	if (!bIsBlocking && UCustodianAbilitySystemLibrary::IsInMeleeStance(GetCustodianCharacter())
		&& ICombatInterface::Execute_GetIsAttacking(GetCustodianCharacter()) == false
		&& bEquippedMeleeWeapon == true)
	{
		bIsBlocking = true;
		ICombatInterface::Execute_SetIsBlocking(GetCustodianCharacter(), true);
		CalculatePlayerCameraDirection();
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FCustodianGameplayTags::Get().Abilities_Stamina_Block);
		CustodianAbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
	}
}

void ACustodianPlayerController::EndBlock()
{
	if (bIsBlocking)
	{
		bIsBlocking = false;
		ICombatInterface::Execute_SetIsBlocking(GetCustodianCharacter(), false);
		ICombatInterface::Execute_CallOnBlockEnded(GetCustodianCharacter());
		UCustodianAbilitySystemLibrary::RegenerateCharacterStamina(GetCustodianCharacter());
	}
}

void ACustodianPlayerController::OpenInventory()
{
	if (!bIsInventoryOpened)
	{
		UCustodianAbilitySystemLibrary::OpenInventory(GetCustodianCharacter());
		return;
	}
	else
	{
		UCustodianAbilitySystemLibrary::CloseInventory(GetCustodianCharacter());
		return;
	}
}

void ACustodianPlayerController::Interact()
{

}

void ACustodianPlayerController::UseConsumableItem()
{
	
}

void ACustodianPlayerController::QuickSave()
{

}

void ACustodianPlayerController::DeleteSave()
{

}

void ACustodianPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FCustodianGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void ACustodianPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FCustodianGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void ACustodianPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FCustodianGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
}

bool ACustodianPlayerController::CanAim()
{
	if (UCustodianAbilitySystemLibrary::IsInMagicStance(GetCustodianCharacter())
	   || (UCustodianAbilitySystemLibrary::IsInRangedStance(GetCustodianCharacter())
		&& ICombatInterface::Execute_GetIsFiring(GetCustodianCharacter()) == false)) return true;
	else return false;
}

void ACustodianPlayerController::AddApplicableMappingContext(UInputMappingContext* InMappingContext)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		if (UCustodianAbilitySystemLibrary::IsInDefaultStance(GetCustodianCharacter())) Subsystem->RemoveMappingContext(CustodianMappingContext);
		if (UCustodianAbilitySystemLibrary::IsInRangedStance(GetCustodianCharacter())) Subsystem->RemoveMappingContext(RangedMappingContext);
		if (UCustodianAbilitySystemLibrary::IsInRangedStance(GetCustodianCharacter()) && !bIsAiming) Subsystem->RemoveMappingContext(AimRangedMappingContext);
		if (UCustodianAbilitySystemLibrary::IsInMagicStance(GetCustodianCharacter())) Subsystem->RemoveMappingContext(MagicMappingContext);
		if (UCustodianAbilitySystemLibrary::IsInMagicStance(GetCustodianCharacter()) && !bIsAiming) Subsystem->RemoveMappingContext(AimMagicMappingContext);
		if (UCustodianAbilitySystemLibrary::IsInMeleeStance(GetCustodianCharacter())) Subsystem->RemoveMappingContext(MeleeMappingContext);
		Subsystem->AddMappingContext(InMappingContext, 0);
	}
}

bool ACustodianPlayerController::CanEnterDesiredStance(EDesiredStance InDesiredStance)
{
	if (!bIsAiming && !bIsBlocking)
	{
		switch (InDesiredStance)
		{
		// Entering Default Stance
		case EDesiredStance::EDS_DefaultStance:
			if (bEquippedRangedWeapon)
			{
				bEquippedRangedWeapon = false;
				ICombatInterface::Execute_UnequipRangedWeapon(GetCustodianCharacter(), false);
				return true;
			}
			if (bEquippedMeleeWeapon)
			{
				bEquippedMeleeWeapon = false;
				ICombatInterface::Execute_UnequipMeleeWeapon(GetCustodianCharacter(), false);
				return true;
			}
			return true;

		// Entering Ranged Stance
		case EDesiredStance::EDS_RangedStance:
			if (bIsWieldingRangedWeapon)
			{
				if (bEquippedMeleeWeapon)
				{
					bEquippedMeleeWeapon = false;
				}
				if (!bEquippedRangedWeapon)
				{
					bEquippedRangedWeapon = true;
					ICombatInterface::Execute_EquipRangedWeapon(GetCustodianCharacter(), true);
					return true;
				}
				return true;
			}
			else return false;

		// Entering Melee Stance
		case EDesiredStance::EDS_MeleeStance:
			if (bIsWieldingMeleeWeapon)
			{
				if (bEquippedRangedWeapon)
				{
					bEquippedRangedWeapon = false;
				}
				if (!bEquippedMeleeWeapon)
				{
					bEquippedMeleeWeapon = true;
					ICombatInterface::Execute_EquipMeleeWeapon(GetCustodianCharacter(), true);
					return true;
				}
				return true;
			}
			else return false;

		// Entering Magic Stance
		case EDesiredStance::EDS_MagicStance:
			if (bEquippedRangedWeapon)
			{
				bEquippedRangedWeapon = false;
				ICombatInterface::Execute_UnequipRangedWeapon(GetCustodianCharacter(), false);
				return true;
			}
			if (bEquippedMeleeWeapon)
			{
				bEquippedMeleeWeapon = false;
				ICombatInterface::Execute_UnequipMeleeWeapon(GetCustodianCharacter(), false);
				return true;
			}
			return true;
		}
	}
	return false;
}

void ACustodianPlayerController::CalculatePlayerCameraDirection()
{
	// Get Current Character Rotation
	FRotator CurrentCharacterRotation = GetCustodianCharacter()->GetActorRotation();
	// Get Current Camera Rotation
	FRotator CurrentCameraRotation = GetControlRotation();

	// Calculate delta rotation and normalize it
	FRotator DeltaRotation = CurrentCameraRotation - CurrentCharacterRotation;
	DeltaRotation.Normalize();

	// Define the attack direction
	if (FMath::Abs(DeltaRotation.Pitch) > FMath::Abs(DeltaRotation.Yaw))
	{
		PlayerCameraDirection = DeltaRotation.Pitch > 0 ? ECameraDirection::BottomUp : ECameraDirection::TopDown;
	}
	else
	{
		PlayerCameraDirection = DeltaRotation.Yaw > 0 ? ECameraDirection::Right : ECameraDirection::Left;
	}
}
