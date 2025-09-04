// Copyright Sertim all rights reserved


#include "UI/Widget/CustodianUserWidget.h"

void UCustodianUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
