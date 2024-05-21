// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"

#include "Aura/AuraGameplayTagsManager.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	auto AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	check(AuraAttributeSet);
	for (const auto& [Tag, Attribute] : AuraAttributeSet->TagToAttribute)
	{
		BroadcastAuraAttributeInfoByGameplayTag(Tag, Attribute());
	}
}

void UAttributeMenuWidgetController::BindCallbackToDependencies()
{
	auto AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	check(AuraAttributeSet);
	for (const auto& [Tag, Attribute] : AuraAttributeSet->TagToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute()).AddLambda([this, &Tag, &Attribute](const FOnAttributeChangeData& Data)
		{
			BroadcastAuraAttributeInfoByGameplayTag(Tag, Attribute());
		});
	}
}

void UAttributeMenuWidgetController::BroadcastAuraAttributeInfoByGameplayTag(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const
{
	if (auto AttributeInfo = AttributeInfoDataAsset->AttributeInfos.Find(Tag))
	{
		AttributeInfo->Value = Attribute.GetNumericValue(AttributeSet);
		AuraAttributeInfoDelegate.Broadcast(*AttributeInfo);
	}
}
