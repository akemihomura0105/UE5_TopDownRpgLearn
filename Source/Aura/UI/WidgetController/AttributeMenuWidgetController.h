// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayAttribute;
struct FGameplayTag;
struct FAuraAttributeInfo;
class UAttributeInfoDataAsset;
class UAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuraAttributeInfoSignature, const FAuraAttributeInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;
	
	virtual void BindCallbackToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FAuraAttributeInfoSignature AuraAttributeInfoDelegate;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAttributeInfoDataAsset> AttributeInfoDataAsset;

	void BroadcastAuraAttributeInfoByGameplayTag(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const;
};
