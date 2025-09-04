// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadScreenWidget.generated.h"

// This widget class is used for creating LoadScreen widget in LoadScreen level
UCLASS()
class CUSTODIAN_API ULoadScreenWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Load Screen")
	void BlueprintInitializeWidget();
};
