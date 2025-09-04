// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

// Interface class that should be used by objects that save in game progress or world state
class CUSTODIAN_API ISaveInterface
{
	GENERATED_BODY()

public:

	// Checks if should load player location
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (ReturnDisplayName = "ShouldLoad"))
	bool ShouldLoadTransform();

	// Call this to load reached actor
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadActor();
};
