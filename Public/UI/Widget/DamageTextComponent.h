// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

// Class created to show Damage text in widgets
UCLASS()
class CUSTODIAN_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	// Sets amount of damage and defines whether it was Blocked or Critical Hit to use different Text color
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bBlockedHit, bool bCriticalHit);
};
