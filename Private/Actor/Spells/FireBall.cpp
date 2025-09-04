// Copyright Sertim all rights reserved


#include "Actor/Spells/FireBall.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "CustodianGameplayTags.h"
#include "Perception/AISense_Damage.h"
#include "AbilitySystemComponent.h"

void AFireBall::BeginPlay()
{
	Super::BeginPlay();

	StartOutgoingTimeline();
}

void AFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromResult, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			// Report Damage Event for AI 
			// P.S. do not put 0 in damage amount!
			AActor* SourceActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
			AActor* TargetActor = TargetASC->GetAvatarActor();
			UAISense_Damage::ReportDamageEvent(GetWorld(), TargetActor, SourceActor, 1.f, SourceActor->GetActorLocation(), TargetActor->GetActorLocation());

			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UCustodianAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

void AFireBall::OnHit()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
}
