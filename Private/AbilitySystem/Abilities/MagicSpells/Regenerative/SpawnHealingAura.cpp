// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/MagicSpells/Regenerative/SpawnHealingAura.h"


FString USpawnHealingAura::GetDescription(int32 Level)
{
    float TotalHealthRegen = CalculateHealthRegeneration(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);

    return FString::Printf(TEXT(
        // Title
        "<Title>Spawn Healing Aura</>\n\n"
        // Level
        "<Small>Level: </><Level>%d</>\n"
        // Mana Cost
        "<Small>Mana Cost: </><ManaCost>%.1f</>\n"
        // Cooldown
        "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
        // Health regeneration
        "<Default>Cast this spell to spawn Healing Aura. While overlapping with it you can "
        "increase your Health by following amount per second: </><Damage>%d </>"),

        Level,
        ManaCost,
        Cooldown,
        static_cast<int32>(TotalHealthRegen)
    );
}

FString USpawnHealingAura::GetNextLevelDescription(int32 Level)
{
    float TotalHealthRegen = CalculateHealthRegeneration(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);

    return FString::Printf(TEXT(
        // Title
        "<Title>Spawn Healing Aura (Next Lvl)</>\n\n"
        // Level
        "<Small>Level: </><Level>%d</>\n"
        // Mana Cost
        "<Small>ManaCost: </><ManaCost>%.1f</>\n"
        // Cooldown
        "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
        // Health regeneration
        "<Default>Cast this spell to spawn Healing Aura. While overlapping with it you can "
        "increase your Health by following amount per second: </><Damage>%d </>"),

        Level,
        ManaCost,
        Cooldown,
        static_cast<int32>(TotalHealthRegen)
    );
}

float USpawnHealingAura::CalculateHealthRegeneration(int32 Level)
{
    if (!AbilityGameplayEffectClass)
    {
        UE_LOG(LogTemp, Error, TEXT("AbilityGameplayEffectClass is nullptr. Please assign it in GA_CustodianSpawnHealingAura ability Blueprint"));
        return 0.f;
    }
    float TotalHealthRegen = 0.f;
    UGameplayEffect* AuraEffect = Cast<UGameplayEffect>(AbilityGameplayEffectClass->GetDefaultObject());
    for (const FGameplayModifierInfo& Modifier : AuraEffect->Modifiers)
    {
        float Magnitude = 0.f;
        Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, Magnitude);
        TotalHealthRegen += Magnitude;
    }
    return TotalHealthRegen;
}
