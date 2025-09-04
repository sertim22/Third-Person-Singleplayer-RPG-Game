// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustodianUserWidget.generated.h"

// Custom User Widget class created to have Widget Controllers to be able to broadcast different values and variables in Widgets
UCLASS()
class CUSTODIAN_API UCustodianUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// Call this function to set Widget Controller for the Widget you are working with
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UObject> WidgetController;

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Widget Controller")
	void WidgetControllerSet();
};
