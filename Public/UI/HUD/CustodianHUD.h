// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CustodianHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UCustodianUserWidget;
class USpellMenuWidgetController;

// Custom HUD class used in Custodian project. Contains different widget controllers and main Overlay Widget
UCLASS()
class CUSTODIAN_API ACustodianHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	/*Getters*/
	UOverlayWidgetController* GetOverlayWidgetControllerParams(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);
	/*Getters end*/

	// Creates main Overlay Widget and sets Widget Controller to broadcast values.
	// Call this function in CustodianCharacter class in "InitAbilityActorInfo" function
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	// Returns Overlay Widget shown on Player's screen
	UFUNCTION(BlueprintCallable, Category = "Widgets|Overlay", meta = (ReturnDisplayName = "OverlayWidget"))
	UCustodianUserWidget* GetCustodianOverlayWidget(ACustodianHUD* InBP_CustodianHUD);

private:

	UPROPERTY()
	TObjectPtr<UCustodianUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere, Category = "Overlay")
	TSubclassOf<UCustodianUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere, Category = "Overlay|Widget Controllers")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere, Category = "Overlay|Widget Controllers")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	UPROPERTY(EditAnywhere, Category = "Overlay|Widget Controllers")
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
};
