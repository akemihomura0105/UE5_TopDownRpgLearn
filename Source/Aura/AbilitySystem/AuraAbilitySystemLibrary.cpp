// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemLibrary.h"

#include <optional>

#include "AuraAbilitySystemComponent.h"
#include "AuraAttributeSet.h"
#include "Aura/Player/AuraPlayerController.h"
#include "Aura/Player/AuraPlayerState.h"
#include "Aura/UI/HUD/AuraHud.h"
#include "Aura/UI/WidgetController/AttributeMenuWidgetController.h"
#include "Aura/UI/WidgetController/AuraWidgetController.h"
#include "Kismet/GameplayStatics.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (auto WidgetParamOpt = ConstructAuraWidgetControllerParams(WorldContextObject))
	{
		auto AuraHud = Cast<AAuraHud>(WidgetParamOpt->PlayerController->GetHUD());
		if (!AuraHud)
		{
			return nullptr;
		}
		return AuraHud->GetOrCreateOverlayWidgetController(*WidgetParamOpt);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (auto WidgetParamOpt = ConstructAuraWidgetControllerParams(WorldContextObject))
	{
		auto AuraHud = Cast<AAuraHud>(WidgetParamOpt->PlayerController->GetHUD());
		if (!AuraHud)
		{
			return nullptr;
		}
		return AuraHud->GetOrCreateAttributeMenuWidgetController(*WidgetParamOpt);
	}
	return nullptr;
}

std::optional<FAuraWidgetControllerParams> UAuraAbilitySystemLibrary::ConstructAuraWidgetControllerParams(const UObject* WorldContextObject)
{
	if (auto PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		auto AuraPlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
		if (!AuraPlayerState)
		{
			return {};
		}
		auto AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent());
		if (!AuraAbilitySystemComponent)
		{
			return {};
		}
		auto AuraAttributeSet = AuraAbilitySystemComponent->GetAttributeSet(UAuraAttributeSet::StaticClass());
		if (!AuraAttributeSet)
		{
			return {};
		}

		FAuraWidgetControllerParams Params;
		Params.PlayerController = PlayerController;
		Params.PlayerState = AuraPlayerState;
		Params.AbilitySystemComponent = AuraAbilitySystemComponent;
		Params.AttributeSet = AuraAttributeSet;
		return Params;
	}
	return {};
}
