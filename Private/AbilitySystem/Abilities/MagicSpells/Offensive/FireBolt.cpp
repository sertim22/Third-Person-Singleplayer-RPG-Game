// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/MagicSpells/Offensive/FireBolt.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Actor/CustodianProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CustodianGameplayTags.h"
#include "Components/SphereComponent.h"


FString UFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Fire Bolt</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>Cast this spell to launch a bolt of Fire, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d </><Default>Fire Damage with"
			" a chance to cause Burning Debuff Effect on the Target</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Fire Bolt</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of FireBolts
			"<Default>Cast this spell to launch %d bolts of Fire, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d </><Default>Fire Damage with"
			" a chance to cause Burning Debuff Effect on the Target</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumProjectiles),
			ScaledDamage);
	}
}

FString UFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		// Title
		"<Title>Fire Bolt (Next Lvl) </>\n\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Number of FireBolts
		"<Default>Cast this spell to launch %d bolts of Fire, "
		"exploding on impact and dealing: </>"

		// Damage
		"<Damage>%d </><Default>Fire Damage with"
		" a chance to cause Burning Debuff Effect on the Target.</>"),

		// Values
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumProjectiles),
		ScaledDamage);
}

void UFireBolt::SpawnFireBolt(const FGameplayTag& SocketTag)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotation = SocketLocation.Rotation();

	const FVector Forward = Rotation.Vector();
	const int32 EffectiveNumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	TArray<FRotator> Rotations = UCustodianAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;

		FVector RandomLocation = FVector(0.f, FMath::RandRange(-30.f, 30.f), FMath::RandRange(-30.f, 30.f));
		SpawnTransform.SetLocation(SocketLocation + RandomLocation);

		SpawnTransform.SetRotation(Rotation.Quaternion());

		ACustodianProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACustodianProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->SetOwner(GetAvatarActorFromActorInfo());
		Projectile->SetInstigator(Cast<APawn>(GetAvatarActorFromActorInfo()));

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		Projectile->FinishSpawning(SpawnTransform);

		Projectile->Sphere->IgnoreActorWhenMoving(GetAvatarActorFromActorInfo(), true);

		// Projectile Direction and Speed
		const FVector ProjectileDirection = CalculateProjectileDirection(Projectile);
		Projectile->ProjectileMovement->Velocity = ProjectileDirection * Projectile->ProjectileMovement->InitialSpeed;
	}
}