// Copyright Sertim all rights reserved


#include "Character/CustodianCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/CustodianAbilitySystemComponent.h"
#include "AbilitySystem/CustodianAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Custodian/Custodian.h"
#include "CustodianGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "Components/BoxComponent.h"


ACustodianCharacterBase::ACustodianCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>(TEXT("BurnDebuffComponent"));
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FCustodianGameplayTags::Get().Debuff_Burn;

	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>(TEXT("StunDebuffComponent"));
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = FCustodianGameplayTags::Get().Debuff_Stun;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);


	/*Effects*/
	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EffectAttachPoint"));
	EffectAttachComponent->SetupAttachment(GetRootComponent());
	PassiveSpellNiagaraComponent_01 = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("PassiveSpellNiagaraComponent_01"));
	PassiveSpellNiagaraComponent_01->SetupAttachment(EffectAttachComponent);
	PassiveSpellNiagaraComponent_02 = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("PassiveSpellNiagaraComponent_02"));
	PassiveSpellNiagaraComponent_02->SetupAttachment(EffectAttachComponent);
	/*Effects end*/


	/*
	* Armor&Equipment
	*/

	HeadGearMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadGearMesh"));
	HeadGearMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadGearMesh->SetupAttachment(GetMesh());
	HeadGearMesh->SetLeaderPoseComponent(GetMesh());

	ChestGearMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChestGearMesh"));
	ChestGearMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChestGearMesh->SetupAttachment(GetMesh());
	ChestGearMesh->SetLeaderPoseComponent(GetMesh());

	ArmsGearMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsGearMesh"));
	ArmsGearMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArmsGearMesh->SetupAttachment(GetMesh());
	ArmsGearMesh->SetLeaderPoseComponent(GetMesh());

	LegsGearMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegsGearMesh"));
	LegsGearMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LegsGearMesh->SetupAttachment(GetMesh());
	LegsGearMesh->SetLeaderPoseComponent(GetMesh());

	RightHandMeleeWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHandMeleeWeaponMesh"));
	RightHandMeleeWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandMeleeWeaponMesh->SetupAttachment(GetMesh(), ScabbardSocketName);

	LeftHandMeleeWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHandMeleeWeaponMesh"));
	LeftHandMeleeWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandMeleeWeaponMesh->SetupAttachment(GetMesh(), ShieldSpineSocketName);

	RightHandWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandWeaponCollision"));
	RightHandWeaponCollision->SetupAttachment(RightHandMeleeWeaponMesh);
	RightHandWeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightHandWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightHandWeaponCollision->ComponentTags.Add(FName("Weapon"));

	RangedWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RangedWeaponMesh"));
	RangedWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RangedWeaponMesh->SetupAttachment(GetMesh(), SpineSocketName);
	
	/*
	* Armor&Equipment
	*/
}

void ACustodianCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
}

float ACustodianCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamageDelegate.Broadcast(DamageTaken);
	return DamageTaken;
}

UAbilitySystemComponent* ACustodianCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ACustodianCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ACustodianCharacterBase::Die(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bDead = true;
	BurnDebuffComponent->Deactivate();
	StunDebuffComponent->Deactivate();
	OnDeathDelegate.Broadcast(this);
}

FOnDeathSignature& ACustodianCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

FVector ACustodianCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag)
{
	const FCustodianGameplayTags& GameplayTags = FCustodianGameplayTags::Get();
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_ArrowSocket))
	{
		return GetMesh()->GetSocketLocation(ArrowSocketName);
	}
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_ProjectileSocket))
	{
		return GetMesh()->GetSocketLocation(ProjectileSocketName);
	}
	return FVector();
}

bool ACustodianCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ACustodianCharacterBase::GetAvatar_Implementation()
{
	return this;
}

UParticleSystem* ACustodianCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

int32 ACustodianCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void ACustodianCharacterBase::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass ACustodianCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered& ACustodianCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered;
}

void ACustodianCharacterBase::SetIsBeingShocked_Implementation(bool bInShock)
{
	bIsBeingShocked = bInShock;
}

bool ACustodianCharacterBase::IsBeingShocked_Implementation() const
{
	return bIsBeingShocked;
}

FOnDamageSignature& ACustodianCharacterBase::GetOnDamageDelegate()
{
	return OnDamageDelegate;
}

bool ACustodianCharacterBase::ReserveAttackToken_Implementation(int32 Amount)
{
	if (AttackTokenCount >= Amount)
	{
		AttackTokenCount = AttackTokenCount - Amount;
		return true;
	}
	else
	{
		return false;
	}
}

void ACustodianCharacterBase::ReturnAttackToken_Implementation(int32 Amount)
{
	AttackTokenCount = AttackTokenCount + Amount; 
}

// Vital Attributes Getters
float ACustodianCharacterBase::GetCharacterHealth_Implementation()
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

float ACustodianCharacterBase::GetCharacterMaxHealth_Implementation()
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

float ACustodianCharacterBase::GetCharacterStamina_Implementation()
{
	if (UCustodianAttributeSet* CustodianAS = Cast<UCustodianAttributeSet>(GetAttributeSet()))
	{
		return CustodianAS->GetStamina();
	}
	else
	{
		return 0.f;
	}
}

float ACustodianCharacterBase::GetCharacterMaxStamina_Implementation()
{
	if (UCustodianAttributeSet* CustodianAS = Cast<UCustodianAttributeSet>(GetAttributeSet()))
	{
		return CustodianAS->GetMaxStamina();
	}
	else
	{
		return 0.f;
	}
}

float ACustodianCharacterBase::GetCharacterMana_Implementation()
{
	if (UCustodianAttributeSet* CustodianAS = Cast<UCustodianAttributeSet>(GetAttributeSet()))
	{
		return CustodianAS->GetMana();
	}
	else
	{
		return 0.f;
	}
}

float ACustodianCharacterBase::GetCharacterMaxMana_Implementation()
{
	if (UCustodianAttributeSet* CustodianAS = Cast<UCustodianAttributeSet>(GetAttributeSet()))
	{
		return CustodianAS->GetMaxMana();
	}
	else
	{
		return 0.f;
	}
}
// end Vital Attributes Getters

// Inventroy Interface
int32 ACustodianCharacterBase::GetCharacterGold_Implementation()
{
	return Gold;
}

void ACustodianCharacterBase::UpdateGoldAmount_Implementation(int32 InGoldAmount)
{
	Gold = Gold + InGoldAmount;
}

void ACustodianCharacterBase::EquipHeadGear_Implementation(UObject* InHeadGearMesh)
{
	if (InHeadGearMesh)
	{
		HeadGearMesh->SetSkeletalMeshAsset(Cast<USkeletalMesh>(InHeadGearMesh));
	}
	if (InHeadGearMesh == nullptr)
	{
		HeadGearMesh->SetSkeletalMeshAsset(nullptr);
	}
}

void ACustodianCharacterBase::EquipChestGear_Implementation(UObject* InChestGearMesh)
{
	if (InChestGearMesh)
	{
		ChestGearMesh->SetSkeletalMeshAsset(Cast<USkeletalMesh>(InChestGearMesh));
	}
	if (InChestGearMesh == nullptr)
	{
		ChestGearMesh->SetSkeletalMeshAsset(nullptr);
	}
}

void ACustodianCharacterBase::EquipArmsGear_Implementation(UObject* InArmsGearMesh)
{
	if (InArmsGearMesh)
	{
		ArmsGearMesh->SetSkeletalMeshAsset(Cast<USkeletalMesh>(InArmsGearMesh));
	}
	if (InArmsGearMesh == nullptr)
	{
		ArmsGearMesh->SetSkeletalMeshAsset(nullptr);
	}
}

void ACustodianCharacterBase::EquipLegsGear_Implementation(UObject* InLegsGearMesh)
{
	if (InLegsGearMesh)
	{
		LegsGearMesh->SetSkeletalMeshAsset(Cast<USkeletalMesh>(InLegsGearMesh));
	}
	if (InLegsGearMesh == nullptr)
	{
		LegsGearMesh->SetSkeletalMeshAsset(nullptr);
	}
}

void ACustodianCharacterBase::EquipRightHandMeleeWeapon_Implementation(UObject* InRightHandMeleeWeaponMesh)
{
	if (InRightHandMeleeWeaponMesh)
	{
		RightHandMeleeWeaponMesh->SetStaticMesh(Cast<UStaticMesh>(InRightHandMeleeWeaponMesh));
	}
	if (InRightHandMeleeWeaponMesh == nullptr)
	{
		RightHandMeleeWeaponMesh->SetStaticMesh(nullptr);
	}
}

void ACustodianCharacterBase::EquipLeftHandMeleeWeapon_Implementation(UObject* InLeftHandMeleeWeaponMesh)
{
	if (InLeftHandMeleeWeaponMesh)
	{
		LeftHandMeleeWeaponMesh->SetStaticMesh(Cast<UStaticMesh>(InLeftHandMeleeWeaponMesh));
	}
	if (InLeftHandMeleeWeaponMesh == nullptr)
	{
		LeftHandMeleeWeaponMesh->SetStaticMesh(nullptr);
	}
}

void ACustodianCharacterBase::EquipRangedWeaponMesh_Implementation(UObject* InRangedWeaponMesh)
{
	if (InRangedWeaponMesh)
	{
		RangedWeaponMesh->SetSkeletalMeshAsset(Cast<USkeletalMesh>(InRangedWeaponMesh));
	}
	if (InRangedWeaponMesh == nullptr)
	{
		RangedWeaponMesh->SetSkeletalMeshAsset(nullptr);
	}
}
// end Inventory Interface

void ACustodianCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACustodianCharacterBase::InitAbilityActorInfo()
{

}

void ACustodianCharacterBase::CheckStamina()
{
	// Set second timer to check if stamina should regen or stop regen
	GetWorld()->GetTimerManager().SetTimer(StaminaCheckTimerHandle,
		[this]()
		{
			bool bShouldStopRegen = false;
			if (bIsAttacking || bIsBlocking || bIsPreAttacking 
				|| ICombatInterface::Execute_GetIsDrawing(this) || GetCharacterMovement()->IsFalling()
				|| bIsSprinting) bShouldStopRegen = true;
			if (bShouldStopRegen)
			{
				AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(
					StaminaRegenerationEffectClass, AbilitySystemComponent);
				GetWorld()->GetTimerManager().ClearTimer(StaminaCheckTimerHandle);
			}
			else
			{
				float CurrentStamina = GetCharacterStamina_Implementation();
				float MaxStamina = GetCharacterMaxStamina_Implementation();
				if (CurrentStamina >= MaxStamina)
				{
					AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(
						StaminaRegenerationEffectClass, AbilitySystemComponent);
					GetWorld()->GetTimerManager().ClearTimer(StaminaCheckTimerHandle);
				}
			}
		}
	, 0.1f, true);
}

void ACustodianCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void ACustodianCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const
{
	check(GetAbilitySystemComponent());
	check(GameplayEffect);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const  FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACustodianCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ACustodianCharacterBase::AddCharacterAbilities()
{
	UCustodianAbilitySystemComponent* CustodianASC = CastChecked<UCustodianAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	CustodianASC->AddCharacterAbilities(StartupAbilities);
	CustodianASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}