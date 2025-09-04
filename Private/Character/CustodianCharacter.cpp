// Copyright Sertim all rights reserved


#include "Character/CustodianCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/CustodianAbilitySystemComponent.h"
#include "Game/CustodianGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/CustodianAttributeSet.h"
#include "Player/CustodianPlayerController.h"
#include "Player/CustodianPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "UI/HUD/CustodianHUD.h"
#include "NiagaraComponent.h"
#include "CustodianGameplayTags.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/CustodianSaveGame.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"


ACustodianCharacter::ACustodianCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Ranger;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpNiagaraComponent"));
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}

void ACustodianCharacter::BeginPlay()
{
	Super::BeginPlay();

	// InitiAbilityActorInfo for the Player
	InitAbilityActorInfo();
	AddCharacterAbilities();
	InitializeDefaultAttributes(); // TODO : убрать InitializeDefaultAttributes
	LoadProgress();
	if (ACustodianGameModeBase* CustodianGameMode = Cast<ACustodianGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		CustodianGameMode->LoadWorldState(GetWorld());
	}
}

void ACustodianCharacter::LoadProgress()
{
	ACustodianGameModeBase* CustodianGameMode = Cast<ACustodianGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (CustodianGameMode)
	{
		UCustodianSaveGame* SaveData = CustodianGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		if (SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			if (UCustodianAbilitySystemComponent* CustodianASC = Cast<UCustodianAbilitySystemComponent>(AbilitySystemComponent))
			{
				CustodianASC->AddCharacterAbilitiesFromSaveData(SaveData);
			}

			if (ACustodianPlayerState* CustodianPlayerState = Cast<ACustodianPlayerState>(GetPlayerState()))
			{
				CustodianPlayerState->SetLevel(SaveData->PlayerLevel);
				CustodianPlayerState->SetXP(SaveData->XP);
				CustodianPlayerState->SetAttributePoints(SaveData->AttributePoints);
				CustodianPlayerState->SetSpellPoints(SaveData->SpellPoints);
			}

			UCustodianAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}

void ACustodianCharacter::AddToXP_Implementation(int32 InXP)
{
	ACustodianPlayerState* CustodianPlayerState = GetPlayerState<ACustodianPlayerState>();
	check(CustodianPlayerState);
	CustodianPlayerState->AddToXP(InXP);
}

void ACustodianCharacter::LevelUp_Implementation()
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = FollowCamera->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
		UGameplayStatics::PlaySound2D(this, LevelUpSound);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				LevelUpNiagaraComponent->Deactivate();
			}, LevelUpEffectDurationTime, false);
	}
}

int32 ACustodianCharacter::GetXP_Implementation() const
{
	const ACustodianPlayerState* CustodianPlayerState = GetPlayerState<ACustodianPlayerState>();
	check(CustodianPlayerState);
	return CustodianPlayerState->GetXP();
}

int32 ACustodianCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const ACustodianPlayerState* CustodianPlayerState = GetPlayerState<ACustodianPlayerState>();
	check(CustodianPlayerState);
	return CustodianPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 ACustodianCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const ACustodianPlayerState* CustodianPlayerState = GetPlayerState<ACustodianPlayerState>();
	check(CustodianPlayerState);
	return CustodianPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointReward;
}

int32 ACustodianCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const ACustodianPlayerState* CustodianPlayerState = GetPlayerState<ACustodianPlayerState>();
	check(CustodianPlayerState);
	return CustodianPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointReward;
}

void ACustodianCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ACustodianPlayerState* CustodianPlayerState = GetPlayerState<ACustodianPlayerState>();
	check(CustodianPlayerState);
	CustodianPlayerState->AddToLevel(InPlayerLevel);

	if (UCustodianAbilitySystemComponent* CustodianASC = Cast<UCustodianAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		CustodianASC->UpdateAbilityStatuses(CustodianPlayerState->GetPlayerLevel());
	}
}

void ACustodianCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	ACustodianPlayerState* CustodianPlayerState = GetPlayerState<ACustodianPlayerState>();
	check(CustodianPlayerState);
	CustodianPlayerState->AddToAttributePoints(InAttributePoints);
}

void ACustodianCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	ACustodianPlayerState* CustodianPlayerState = GetPlayerState<ACustodianPlayerState>();
	check(CustodianPlayerState);
	CustodianPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 ACustodianCharacter::GetAttributePoints_Implementation() const
{
	ACustodianPlayerState* CustodianPlayerState = GetPlayerState<ACustodianPlayerState>();
	check(CustodianPlayerState);
	return CustodianPlayerState->GetAttributePoints();
}

int32 ACustodianCharacter::GetSpellPoints_Implementation() const
{
	ACustodianPlayerState* CustodianPlayerState = GetPlayerState<ACustodianPlayerState>();
	check(CustodianPlayerState);
	return CustodianPlayerState->GetSpellPoints();
}

void ACustodianCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	ACustodianGameModeBase* CustodianGameMode = Cast<ACustodianGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (CustodianGameMode)
	{
		UCustodianSaveGame* SaveData = CustodianGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		SaveData->PlayerStartTag = CheckpointTag;

		if (ACustodianPlayerState* CustodianPlayerState = Cast<ACustodianPlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel = CustodianPlayerState->GetPlayerLevel();
			SaveData->XP = CustodianPlayerState->GetXP();
			SaveData->AttributePoints = CustodianPlayerState->GetAttributePoints();
			SaveData->SpellPoints = CustodianPlayerState->GetSpellPoints();
		}
		SaveData->Strength = UCustodianAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UCustodianAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Dexterity = UCustodianAttributeSet::GetDexterityAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Constitution = UCustodianAttributeSet::GetConstitutionAttribute().GetNumericValue(GetAttributeSet());

		SaveData->bFirstTimeLoadIn = false;

		if (!HasAuthority()) return;

		UCustodianAbilitySystemComponent* CustodianASC = Cast<UCustodianAbilitySystemComponent>(AbilitySystemComponent);
		FForEachAbility SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();
		SaveAbilityDelegate.BindLambda([this, CustodianASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
			{
				const FGameplayTag AbilityTag = CustodianASC->GetAbilityTagFromSpec(AbilitySpec);
				UAbilityInfo* AbilityInfo = UCustodianAbilitySystemLibrary::GetAbilityInfo(this);
				FCustodianAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);

				FSavedAbility SavedAbility;
				SavedAbility.GameplayAbility = Info.Ability;
				SavedAbility.AbilityLevel = AbilitySpec.Level;
				SavedAbility.AbilitySlot = CustodianASC->GetSlotFromAbilityTag(AbilityTag);
				SavedAbility.AbilityStatus = CustodianASC->GetStatusFromAbilityTag(AbilityTag);
				SavedAbility.AbilityTag = AbilityTag;
				SavedAbility.AbilityType = Info.AbilityType;

				SaveData->SavedAbilities.AddUnique(SavedAbility);
			});
		CustodianASC->ForEachAbility(SaveAbilityDelegate);

		CustodianGameMode->SaveInGameProgressData(SaveData);
	}
}

int32 ACustodianCharacter::GetCharacterLevel_Implementation()
{
	const ACustodianPlayerState* CustodianPlayerState = GetPlayerState<ACustodianPlayerState>();
	if (CustodianPlayerState)
	{
		return CustodianPlayerState->GetPlayerLevel();
	}
	else return 0;
}

void ACustodianCharacter::HealCharacter_Implementation(float HealPercentage)
{
	float Health = ICombatInterface::Execute_GetCharacterHealth(this);
	Health = FMath::Clamp(HealPercentage * ICombatInterface::Execute_GetCharacterMaxHealth(this) + Health, 0.f, ICombatInterface::Execute_GetCharacterMaxHealth(this));
	if (UCustodianAttributeSet* CustodianAS = Cast<UCustodianAttributeSet>(GetAttributeSet()))
	{
		CustodianAS->SetHealth(Health);
	}
}

void ACustodianCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);

	FTimerDelegate DeathTimerDelegate;
	DeathTimerDelegate.BindLambda([this]()
		{
			ACustodianGameModeBase* CustodianGM = Cast<ACustodianGameModeBase>(UGameplayStatics::GetGameMode(this));
			if (CustodianGM)
			{
				CustodianGM->PlayerDied(this);
			}
		});
	GetWorldTimerManager().SetTimer(DeathTimer, DeathTimerDelegate, DeathTime, false);
	FollowCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void ACustodianCharacter::InitAbilityActorInfo()
{
	ACustodianPlayerState* CustodianPlayerState = GetPlayerState<ACustodianPlayerState>();
	check(CustodianPlayerState);
	CustodianPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CustodianPlayerState, this);
	Cast<UCustodianAbilitySystemComponent>(CustodianPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = CustodianPlayerState->GetAbilitySystemComponent();
	AttributeSet = CustodianPlayerState->GetAttributeSet();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FCustodianGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ACustodianCharacter::StunTagChanged);
	
	if (ACustodianPlayerController* CustodianPlayerController = Cast<ACustodianPlayerController>(GetController()))
	{
		if (ACustodianHUD* CustodianHUD = Cast<ACustodianHUD>(CustodianPlayerController->GetHUD()))
		{
			CustodianHUD->InitOverlay(CustodianPlayerController, CustodianPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

