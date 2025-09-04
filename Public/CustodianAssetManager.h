// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CustodianAssetManager.generated.h"

// Custom asset manager class used to Initialize CustodianGameplayTags
UCLASS()
class CUSTODIAN_API UCustodianAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	static UCustodianAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
