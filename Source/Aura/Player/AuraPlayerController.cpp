// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraPlayerController.h"

#include "AuraPlayerState.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Aura/Interface/EnemyInterface.h"
#include "Aura/UI/WidgetController/AuraWidgetController.h"
#include "Aura/UI/HUD/AuraHud.h"
#include "Aura/UI/WidgetController/AuraWidgetControllerParams.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
	if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetHideCursorDuringCapture(false);
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitHUD();
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (auto ControlledPawn = GetPawn<APawn>())
	{
		const auto InputAxisVector = InputActionValue.Get<FVector2D>();
		const FRotator Rotation = GetControlRotation();
		const auto YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}
	TScriptInterface<IEnemyInterface> ThisActorInterface = CursorHit.GetActor();
	if (LastHighLightActor == ThisActorInterface)
	{
		return;
	}
	if (LastHighLightActor != nullptr)
	{
		LastHighLightActor->SetActorHighlight(false);
	}
	if (ThisActorInterface != nullptr)
	{
		ThisActorInterface->SetActorHighlight(true);
	}
	LastHighLightActor = ThisActorInterface;
}

void AAuraPlayerController::InitHUD()
{
	FAuraWidgetControllerParams Params;
	auto PS = GetPlayerState<AAuraPlayerState>();
	check(PS);
	Params.PlayerController = this;
	Params.PlayerState = PS;
	Params.AbilitySystemComponent = PS->GetAbilitySystemComponent();
	Params.AttributeSet = PS->GetAttributeSet();
	auto AuraHud = Cast<AAuraHud>(GetHUD());
	AuraHud->InitOverlay(Params);
}
