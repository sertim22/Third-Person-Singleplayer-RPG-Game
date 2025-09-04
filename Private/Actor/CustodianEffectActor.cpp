// Copyright Sertim all rights reserved


#include "Actor/CustodianEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystem/CustodianAttributeSet.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Kismet/GameplayStatics.h"


ACustodianEffectActor::ACustodianEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));
}

void ACustodianEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void ACustodianEffectActor::ApplyEffectToTarget(AActor* TargetActor, const FEffectType& GameplayEffectClass)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass.GameplayEffect);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass.GameplayEffect, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && GameplayEffectClass.RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}

	// TODO : Make some duration effects undestructable
	if (!bIsInfinite && bDestroyOnEffectApplication)
	{
		Destroy();
	}
}

void ACustodianEffectActor::OnOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	if (bIsDamageEffect)
	{
		if (!IsValidOverlap(TargetActor)) return;
		if (HasAuthority())
		{
			if (OnOverlapSound) UGameplayStatics::PlaySoundAtLocation(this, OnOverlapSound, this->GetActorLocation());
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
			{
				const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
				DamageEffectParams.DeathImpulse = DeathImpulse;

				const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
				if (bKnockback)
				{
					FRotator Rotation = GetActorRotation();
					Rotation.Pitch = 45.f;

					const FVector KnockbackDirection = Rotation.Vector();
					const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
					DamageEffectParams.KnockbackForce = KnockbackForce;
				}

				DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
				UCustodianAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);

				// Report Damage Event for AI 
				// P.S. do not put 0 in damage amount!
				AActor* SourceActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
				AActor* TargetActor = TargetASC->GetAvatarActor();
				UAISense_Damage::ReportDamageEvent(GetWorld(), TargetActor, SourceActor, 1.f, SourceActor->GetActorLocation(), TargetActor->GetActorLocation());
			}
			Destroy();
		}
	}
	else
	{
		for (const FEffectType& Effect : Effects)
		{
			if (Effect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
			{
				if (OnOverlapSound) UGameplayStatics::PlaySoundAtLocation(this, OnOverlapSound, this->GetActorLocation());
				ApplyEffectToTarget(TargetActor, Effect);
			}
		}
	}
}

void ACustodianEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	for (const FEffectType& Effect : Effects)
	{
		if (Effect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, Effect);
		}
		if (Effect.GameplayEffect.GetDefaultObject()->DurationPolicy == EGameplayEffectDurationType::Infinite)
		{
			if (Effect.RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
			{
				UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
				if (!IsValid(TargetASC)) return;

				TArray<FActiveGameplayEffectHandle> HandlesToRemove;
				for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
				{
					if (TargetASC == HandlePair.Value)
					{
						TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
						HandlesToRemove.Add(HandlePair.Key);
					}
				}
				for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
				{
					ActiveEffectHandles.FindAndRemoveChecked(Handle);
				}
			}
		}
	}
}

bool ACustodianEffectActor::IsValidOverlap(AActor* OtherActor)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return false;
	return true;
}


