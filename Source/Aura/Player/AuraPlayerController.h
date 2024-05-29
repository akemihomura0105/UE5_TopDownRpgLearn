// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class IEnemyInterface;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void OnRep_PlayerState() override;
	
private:
	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();
	void TickAutoMove();
	void RefreshHighlightTrace();
	void InitHUD();
	void OnAbilityInputPressed(FGameplayTag InputTag);
	void OnAbilityInputHeld(FGameplayTag InputTag);
	void OnAbilityInputReleased(FGameplayTag InputTag);
	TObjectPtr<UAuraAbilitySystemComponent> GetAuraAbilitySystemComponent();
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	TScriptInterface<IEnemyInterface> LastHighLightActor;

	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	FHitResult CursorHitResult;
	float HoldDuration = 0.f;

	UPROPERTY()
	TObjectPtr<AActor> CursorTarget;
	
	UPROPERTY(EditDefaultsOnly, Category="Move")
	float FinishMoveRadius = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category="Move")
	float ShortClickTime = 0.1f;

	FVector CachedMoveDestination = FVector::ZeroVector;

	bool bIsNeedAutoMove;
	int32 AutoMoveIndex = 0;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USplineComponent> MoveSplineComponent;
};
