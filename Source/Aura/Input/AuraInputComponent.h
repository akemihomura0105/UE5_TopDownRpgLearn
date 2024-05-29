// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAuraInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	template <class ClassType, class PressedFuncType, class HeldFuncType, class ReleasedFuncType>
	void BindAbilityInputActionDispatchCallBack(const UAuraInputConfig* AuraInputConfig, ClassType* BindClass, PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc);
};

template <typename ClassType, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
void UAuraInputComponent::BindAbilityInputActionDispatchCallBack(const UAuraInputConfig* AuraInputConfig, ClassType* BindClass, PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc)
{
	check(AuraInputConfig);
	for (auto [Tag, InputAction] : AuraInputConfig->AbilityInputConfig)
	{
		check(InputAction);
		auto TryBindFunc = [&](ETriggerEvent TriggerType, auto Func)
		{
			if (Func)
			{
				BindAction(InputAction, TriggerType, BindClass, Func, Tag);
			}
		};
		TryBindFunc(ETriggerEvent::Started, PressedFunc);
		TryBindFunc(ETriggerEvent::Triggered, HeldFunc);
		TryBindFunc(ETriggerEvent::Completed, ReleasedFunc);
	}
}
