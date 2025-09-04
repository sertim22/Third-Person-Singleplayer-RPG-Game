// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustodianGameInstance.generated.h"

UCLASS()
class CUSTODIAN_API UCustodianGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex = 0;
};
