// Copyright Sertim all rights reserved


#include "Character/CustodianEnemy.h"
#include "AbilitySystem/CustodianAbilitySystemComponent.h"
#include "AbilitySystem/CustodianAttributeSet.h"
#include "Custodian/Custodian.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/CustodianUserWidget.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "CustodianGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/CustodianAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

ACustodianEnemy::ACustodianEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AbilitySystemComponent = CreateDefaultSubobject<UCustodianAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	AttributeSet = CreateDefaultSubobject<UCustodianAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BaseWalkSpeed = 250.f;
}

void ACustodianEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	CustodianAIController = Cast<ACustodianAIController>(GetController());
	CustodianAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	CustodianAIController->RunBehaviorTree(BehaviorTree);
	CustodianAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	CustodianAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Ranger);
}

void ACustodianEnemy::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void ACustodianEnemy::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
}

void ACustodianEnemy::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	// Do not change OutDestination
}

int32 ACustodianEnemy::GetCharacterLevel_Implementation()
{
	return Level;
}

float ACustodianEnemy::GetCharacterHealth_Implementation()
{
	if (UCustodianAttributeSet* CustodianAS = Cast<UCustodianAttributeSet>(GetAttributeSet()))
	{
		return CustodianAS->GetHealth();
	}
	else
	{
		return 0.f;
	}
}

float ACustodianEnemy::GetCharacterMaxHealth_Implementation()
{
	if (UCustodianAttributeSet* CustodianAS = Cast<UCustodianAttributeSet>(GetAttributeSet()))
	{
		return CustodianAS->GetMaxHealth();
	}
	else
	{
		return 0.f;
	}
}

void ACustodianEnemy::HealCharacter_Implementation(float HealPercentage)
{
	float Health = ICombatInterface::Execute_GetCharacterHealth(this);
	Health = FMath::Clamp(HealPercentage * ICombatInterface::Execute_GetCharacterMaxHealth(this) + Health, 0.f, ICombatInterface::Execute_GetCharacterMaxHealth(this));
	if (UCustodianAttributeSet* CustodianAS = Cast<UCustodianAttributeSet>(GetAttributeSet()))
	{
		CustodianAS->SetHealth(Health);
	}
}

void ACustodianEnemy::Die(const FVector& DeathImpulse)
{
	IEnemyInterface::Execute_ReturnAttackTokensOnDeath(this);
	SetLifeSpan(LifeSpan);
	if (CustodianAIController) CustodianAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	SpawnLoot();
	Super::Die(DeathImpulse);
}

void ACustodianEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ACustodianEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

bool ACustodianEnemy::IsCombatTargetAttacking_Implementation()
{
	if (CombatTarget)
	{
		return ICombatInterface::Execute_GetIsPreAttacking(CombatTarget) ||
			   ICombatInterface::Execute_GetIsAttacking(CombatTarget);
	}
	return false;
}

void ACustodianEnemy::HitReactChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (CustodianAIController && CustodianAIController->GetBlackboardComponent())
	{
		CustodianAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void ACustodianEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();

	if (HasAuthority())
	{
		UCustodianAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	if (UCustodianUserWidget* CustodianUserWidget = Cast<UCustodianUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		CustodianUserWidget->SetWidgetController(this);
	}

	if (const UCustodianAttributeSet* CustodianAS = CastChecked<UCustodianAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CustodianAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CustodianAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FCustodianGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&ACustodianEnemy::HitReactChanged
		);


		OnHealthChanged.Broadcast(CustodianAS->GetHealth());
		OnMaxHealthChanged.Broadcast(CustodianAS->GetMaxHealth());
	}
}

void ACustodianEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->UAbilitySystemComponent::InitAbilityActorInfo(this, this);
	Cast<UCustodianAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FCustodianGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ACustodianEnemy::StunTagChanged);

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	OnASCRegistered.Broadcast(AbilitySystemComponent);
}

void ACustodianEnemy::InitializeDefaultAttributes() const
{
	UCustodianAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void ACustodianEnemy::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);

	if (CustodianAIController && CustodianAIController->GetBlackboardComponent())
	{
		CustodianAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
	}
}
