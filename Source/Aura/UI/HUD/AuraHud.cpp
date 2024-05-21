// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraHud.h"

#include "Blueprint/UserWidget.h"
#include "Aura/UI/Widget/AuraUserWidget.h"
#include "Aura/UI/WidgetController/AttributeMenuWidgetController.h"
#include "Aura/UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHud::GetOrCreateOverlayWidgetController(const FAuraWidgetControllerParams& Params)
{
	if(!OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetAuraWidgetControllerParam(Params);
		OverlayWidgetController->BindCallbackToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHud::GetOrCreateAttributeMenuWidgetController(const FAuraWidgetControllerParams& Params)
{
	if(!AttributeMenuWidgetController)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetAuraWidgetControllerParam(Params);
		AttributeMenuWidgetController->BindCallbackToDependencies();
	}
	return AttributeMenuWidgetController;
}

void AAuraHud::InitOverlay(const FAuraWidgetControllerParams& Params)
{
	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass, "OverlayWidget");
	auto WidgetController = GetOrCreateOverlayWidgetController(Params);
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();	
}

void AAuraHud::BeginPlay()
{
	Super::BeginPlay();

}
