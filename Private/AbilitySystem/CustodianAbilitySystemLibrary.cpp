// Copyright Sertim all rights reserved


#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "Game/CustodianGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CustodianPlayerState.h"
#include "UI/HUD/CustodianHUD.h"
#include "CustodianAbilityTypes.h"
#include "Interaction/CombatInterface.h"
#include "UI/WidgetController/CustodianWidgetController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CustodianGameplayTags.h"
#include "Game/CustodianSaveGame.h"
#include "Player/CustodianPlayerController.h"
#include "Character/CustodianCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "TimerManager.h"


FGameplayAbilitySpecHandle UCustodianAbilitySystemLibrary::GetAbilitySpecHandleByTag(FGameplayTag InAbilityTag, UAbilitySystemComponent* InASC)
{
	FGameplayTagContainer TagsToSearch;
	TagsToSearch.AddTag(InAbilityTag);
	
	if (InASC)
	{
		TArray<FGameplayAbilitySpec*> Abilities;
		InASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagsToSearch, Abilities);
		if (Abilities.Num() > 0)
		{
			return Abilities[0]->Handle;
		}
	}
	return FGameplayAbilitySpecHandle();
}

bool UCustodianAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ACustodianHUD*& OutCustodianHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutCustodianHUD = Cast<ACustodianHUD>(PC->GetHUD());
		if (OutCustodianHUD)
		{
			ACustodianPlayerState* PS = PC->GetPlayerState<ACustodianPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UCustodianAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ACustodianHUD* CustodianHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, CustodianHUD))
	{
		return CustodianHUD->GetOverlayWidgetControllerParams(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UCustodianAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ACustodianHUD* CustodianHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, CustodianHUD))
	{
		return CustodianHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UCustodianAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ACustodianHUD* CustodianHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, CustodianHUD))
	{
		return CustodianHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UCustodianAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UCustodianAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, UCustodianSaveGame* SaveGame)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	const FCustodianGameplayTags& GameplayTags = FCustodianGameplayTags::Get();
	const AActor* SourceAvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Strength, SaveGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Intelligence, SaveGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Dexterity, SaveGame->Dexterity);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Constitution, SaveGame->Constitution);

	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite, 1.f, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UCustodianAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (auto AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetCharacterLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UCustodianAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ACustodianGameModeBase* CustodianGameMode = Cast<ACustodianGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (CustodianGameMode == nullptr) return nullptr;
	return CustodianGameMode->CharacterClassInfo;
}

UAbilityInfo* UCustodianAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const ACustodianGameModeBase* CustodianGameMode = Cast<ACustodianGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (CustodianGameMode == nullptr) return nullptr;
	return CustodianGameMode->AbilityInfo;
}

ULootTiers* UCustodianAbilitySystemLibrary::GetLootTiers(const UObject* WorldContextObject)
{
	const ACustodianGameModeBase* CustodianGameMode = Cast<ACustodianGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (CustodianGameMode == nullptr) return nullptr;
	return CustodianGameMode->LootTiers;
}

bool UCustodianAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CustodianEffectContext->IsBlockedHit();
	}
	return false;
}

bool UCustodianAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CustodianEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UCustodianAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CustodianEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UCustodianAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CustodianEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UCustodianAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CustodianEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UCustodianAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (CustodianEffectContext->GetDamageType().IsValid())
		{
			return *CustodianEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UCustodianAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CustodianEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UCustodianAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CustodianEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

bool UCustodianAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CustodianEffectContext->IsCriticalHit();
	}
	return false;
}

bool UCustodianAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CustodianEffectContext->IsRadialDamage();
	}
	return false;
}

float UCustodianAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CustodianEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UCustodianAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CustodianEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UCustodianAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<const FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return CustodianEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UCustodianAbilitySystemLibrary::SetIsBlockedHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CustodianEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UCustodianAbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CustodianEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UCustodianAbilitySystemLibrary::SetIsSuccessfulDebuff(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CustodianEffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

void UCustodianAbilitySystemLibrary::SetDebuffDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CustodianEffectContext->SetDebuffDamage(InDamage);
	}
}

void UCustodianAbilitySystemLibrary::SetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CustodianEffectContext->SetDebuffDuration(InDuration);
	}
}

void UCustodianAbilitySystemLibrary::SetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CustodianEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void UCustodianAbilitySystemLibrary::SetDamageType(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		CustodianEffectContext->SetDamageType(DamageType);
	}
}

void UCustodianAbilitySystemLibrary::SetDeathImpulse(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CustodianEffectContext->SetDeathImpulse(InImpulse);
	}
}

void UCustodianAbilitySystemLibrary::SetKnockbackForce(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CustodianEffectContext->SetKnockbackForce(InKnockbackForce);
	}
}

void UCustodianAbilitySystemLibrary::SetIsRadialDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CustodianEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UCustodianAbilitySystemLibrary::SetRadialDamageInnerRadius(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CustodianEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UCustodianAbilitySystemLibrary::SetRadialDamageOuterRadius(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CustodianEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UCustodianAbilitySystemLibrary::SetRadialDamageOrigin(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin)
{
	if (FCustodianGameplayEffectContext* CustodianEffectContext = static_cast<FCustodianGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		CustodianEffectContext->SetRadialDamageOrigin(InOrigin);
	}
}

void UCustodianAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}

	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;

	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for (AActor* PotentialTarget : ActorsToCheck)
		{
			const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = PotentialTarget;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutClosestTargets.AddUnique(ClosestActor);
		++NumTargetsFound;
	}
}

FGameplayEffectContextHandle UCustodianAbilitySystemLibrary::ApplyDamageEffect(FDamageEffectParams DamageEffectParams)
{
	const FCustodianGameplayTags& GameplayTags = FCustodianGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);

	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

TArray<FRotator> UCustodianAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, FVector::UpVector);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UCustodianAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, FVector::UpVector);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}

int32 UCustodianAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

FVector UCustodianAbilitySystemLibrary::GetCameraPointLocation(ACharacter* InCharacter, float MaxTrace)
{
	FVector Direction = FVector();

	UCameraComponent* Camera = InCharacter->GetComponentByClass<UCameraComponent>();
	const FVector Start = Camera->K2_GetComponentLocation();
	const FVector End = Camera->GetForwardVector() * MaxTrace + Start;

	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;

	bool bSelectVector = UKismetSystemLibrary::LineTraceSingle(
		InCharacter,
		Start,
		End,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	Direction = bSelectVector ? HitResult.Location : HitResult.TraceEnd;
	//DrawDebugSphere(InCharacter->GetWorld(), Direction, 200.f, 12, FColor::Red, false, 5.f);
	return Direction;
}

ECameraDirection UCustodianAbilitySystemLibrary::GetPlayerCameraDirection(AController* InController)
{
	if (InController)
	{
		return Cast<ACustodianPlayerController>(InController)->PlayerCameraDirection;
	}
	return ECameraDirection::None;
}

void UCustodianAbilitySystemLibrary::RegenerateCharacterStamina(AActor* Owner)
{
	if (!Owner) return;
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	
	// Create Regen Gameplay Effect
	FGameplayEffectContextHandle ContextHandle;
	const FGameplayEffectSpecHandle& EffectSpecHandle = OwnerASC->MakeOutgoingSpec(
		ICombatInterface::Execute_GetStaminaRegenEffectClass(Owner),
		ICombatInterface::Execute_GetCharacterLevel(Owner), ContextHandle);

	// Delay for a short time before applying Regen GE
	FTimerManager& TimerManager = Owner->GetWorld()->GetTimerManager();
	FTimerHandle EffectTimerHandle;
	TimerManager.SetTimer(EffectTimerHandle,
		[OwnerASC, EffectSpecHandle]()
		{
			OwnerASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	, 1.f, false);

	// Call the function that will set looping Timer to check if Stamina Regen Effect should be removed
	ICombatInterface::Execute_CheckCharacterStamina(Owner);
}

ECharacterStance UCustodianAbilitySystemLibrary::GetCharacterStance(ACharacter* InCharacter)
{
	if (ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter))
	{
		return CustodianCharacterBase->CharacterStance;
	}
	else return ECharacterStance();
}

void UCustodianAbilitySystemLibrary::SetCharacterStanceToMagic(ACharacter* InCharacter)
{
	ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter);
	CustodianCharacterBase->CharacterStance = ECharacterStance::ECS_MagicStance;
}

void UCustodianAbilitySystemLibrary::SetCharacterStanceToMelee(ACharacter* InCharacter)
{
	ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter);
	CustodianCharacterBase->CharacterStance = ECharacterStance::ECS_MeleeStance;
}

void UCustodianAbilitySystemLibrary::SetCharacterStanceToRanged(ACharacter* InCharacter)
{
	ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter);
	CustodianCharacterBase->CharacterStance = ECharacterStance::ECS_RangedStance;
}

void UCustodianAbilitySystemLibrary::SetCharacterStanceToDefault(ACharacter* InCharacter)
{
	ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter);
	CustodianCharacterBase->CharacterStance = ECharacterStance::ECS_DefaultStance;
}

bool UCustodianAbilitySystemLibrary::IsInMagicStance(ACharacter* InCharacter)
{
	ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter);
	if (CustodianCharacterBase->CharacterStance == ECharacterStance::ECS_MagicStance)
	{
		return true;
	}
	else return false;
}

bool UCustodianAbilitySystemLibrary::IsInRangedStance(ACharacter* InCharacter)
{
	ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter);
	if (CustodianCharacterBase->CharacterStance == ECharacterStance::ECS_RangedStance)
	{
		return true;
	}
	else return false;
}

bool UCustodianAbilitySystemLibrary::IsInDefaultStance(ACharacter* InCharacter)
{
	ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter);
	if (CustodianCharacterBase->CharacterStance == ECharacterStance::ECS_DefaultStance)
	{
		return true;
	}
	else return false;
}

bool UCustodianAbilitySystemLibrary::IsInMeleeStance(ACharacter* InCharacter)
{
	ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter);
	if (CustodianCharacterBase->CharacterStance == ECharacterStance::ECS_MeleeStance)
	{
		return true;
	}
	else return false;
}

void UCustodianAbilitySystemLibrary::ExitTeleportStance(ACharacter* InCharacter)
{
	// Set Character Movement to walking mode
	UCharacterMovementComponent* CCMovement = InCharacter->GetCharacterMovement();
	CCMovement->SetMovementMode(EMovementMode::MOVE_Walking);
	CCMovement->MaxAcceleration = 1500.f;

	// Make Character mesh visible
	USkeletalMeshComponent* CCMesh = InCharacter->GetMesh();
	CCMesh->SetVisibility(true, true);
	InCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void UCustodianAbilitySystemLibrary::OpenInventory(ACharacter* InCharacter)
{
	bool bInventoryState = GetCustodianPC(InCharacter)->bIsInventoryOpened;
	if (!bInventoryState)
	{
		GetCustodianPC(InCharacter)->bIsInventoryOpened = true;
		UCustodianAbilitySystemLibrary::BroadcastInventoryState(InCharacter, true);
		IInventoryInterface::Execute_OpenInventory(InCharacter);
	}
}

void UCustodianAbilitySystemLibrary::CloseInventory(ACharacter* InCharacter)
{
	bool bInventoryState = GetCustodianPC(InCharacter)->bIsInventoryOpened;
	if (bInventoryState)
	{
		GetCustodianPC(InCharacter)->bIsInventoryOpened = false;
		UCustodianAbilitySystemLibrary::BroadcastInventoryState(InCharacter, false);
		IInventoryInterface::Execute_CloseInventory(InCharacter);
	}
}

bool UCustodianAbilitySystemLibrary::GetInventoryState(ACharacter* InCharacter)
{
	return GetCustodianPC(InCharacter)->bIsInventoryOpened;
}


bool UCustodianAbilitySystemLibrary::IsAiming(ACharacter* InCharacter)
{
	if (GetCustodianPC(InCharacter)->bIsAiming) return true;
	else return false;
}

bool UCustodianAbilitySystemLibrary::IsFiring(ACharacter* InCharacter)
{
	if (GetCustodianPC(InCharacter)->bIsFiring) return true;
	else return false;
}

bool UCustodianAbilitySystemLibrary::IsBlocking(ACharacter* InCharacter)
{
	if (InCharacter)
	{
		ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter);
		if (CustodianCharacterBase->bIsBlocking == true) return true;
		else return false;
	}
	else return false;
}

bool UCustodianAbilitySystemLibrary::IsPreAttacking(ACharacter* InCharacter)
{
	if (InCharacter)
	{
		ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter);
		if (CustodianCharacterBase->bIsPreAttacking == true) return true;
		else return false;
	}
	else return false;
}

bool UCustodianAbilitySystemLibrary::IsAttacking(ACharacter* InCharacter)
{
	if (InCharacter)
	{
		ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter);
		if (CustodianCharacterBase->bIsAttacking == true) return true;
		else return false;
	}
	else return false;
}

bool UCustodianAbilitySystemLibrary::IsEncumbered(ACharacter* InCharacter)
{
	if (InCharacter)
	{
		ACustodianCharacterBase* CustodianCharacterBase = Cast<ACustodianCharacterBase>(InCharacter);
		if (CustodianCharacterBase->bIsEncumbered == true) return true;
		else return false;
	}
	else return false;
}

void UCustodianAbilitySystemLibrary::BroadcastCharacterStance(ECharacterStance InCharacterStance, const UObject* WorldContextObject)
{
	GetOverlayWidgetController(WorldContextObject)->OnCharacterStanceChangedDelegate.Broadcast(InCharacterStance);
}

void UCustodianAbilitySystemLibrary::BroadcastInventoryState(const UObject* WorldContextObject, bool bInventoryState)
{
	GetOverlayWidgetController(WorldContextObject)->OnInventoryStateDelegate.Broadcast(bInventoryState);
}

void UCustodianAbilitySystemLibrary::SetIsRadialDamageEffectParam(UPARAM(ref)FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bIsRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void UCustodianAbilitySystemLibrary::SetKnockbackDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
	}
	else
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;
	}
}

void UCustodianAbilitySystemLibrary::SetDeathImpulseDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;

	}
	else
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
	}
}

void UCustodianAbilitySystemLibrary::SetTargetEffectParamsASC(UPARAM(ref)FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InASC;
}

ACustodianPlayerController* UCustodianAbilitySystemLibrary::GetCustodianPC(ACharacter* InCharacter)
{
	ACustodianPlayerController* CustodianPC = Cast<ACustodianPlayerController>(InCharacter->GetController());
	return CustodianPC;
}
