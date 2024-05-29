// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraPlayerController.h"

#include "AbilitySystemGlobals.h"
#include "AuraPlayerState.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Aura/AuraGameplayTagsManager.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Input/AuraInputComponent.h"
#include "Aura/Interface/EnemyInterface.h"
#include "Aura/UI/HUD/AuraHud.h"
#include "Aura/UI/WidgetController/AuraWidgetControllerParams.h"
#include "Components/SplineComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	MoveSplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
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

	auto AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityInputActionDispatchCallBack(InputConfig, this, &AAuraPlayerController::OnAbilityInputPressed, &AAuraPlayerController::OnAbilityInputHeld, &AAuraPlayerController::OnAbilityInputReleased);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	TickAutoMove();
}

void AAuraPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitHUD();
}

// ReSharper disable once CppMemberFunctionMayBeConst
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
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
	CursorTarget = CursorHitResult.GetActor();
	RefreshHighlightTrace();
}

void AAuraPlayerController::TickAutoMove()
{
	if (!bIsNeedAutoMove)
	{
		return;
	}
	auto ControlPawn = GetPawn();
	if (!ControlPawn)
	{
		return;
	}
	if (AutoMoveIndex >= MoveSplineComponent->GetNumberOfSplinePoints())
	{
		bIsNeedAutoMove = false;
		return;
	}

	auto MoveTargetLocation = MoveSplineComponent->GetWorldLocationAtSplinePoint(AutoMoveIndex);
	auto MoveLine = MoveTargetLocation - ControlPawn->GetTargetLocation();
	if (MoveLine.Size2D() < FinishMoveRadius)
	{
		AutoMoveIndex++;
		TickAutoMove();
	}

	auto MoveDirection = MoveLine.GetSafeNormal2D();
	DrawDebugSphere(GetWorld(), MoveTargetLocation, 10, 4, FColor::Blue);
	for (int32 i = 0; i < MoveSplineComponent->GetNumberOfSplinePoints(); i++)
	{
		DrawDebugSphere(GetWorld(), MoveSplineComponent->GetWorldLocationAtSplinePoint(i), 10, 4, FColor::Black);
	}
	ControlPawn->AddMovementInput(MoveDirection);

	if ((ControlPawn->GetTargetLocation() - CachedMoveDestination).Size2D() < FinishMoveRadius)
	{
		bIsNeedAutoMove = false;
	}
}

void AAuraPlayerController::RefreshHighlightTrace()
{
	TScriptInterface<IEnemyInterface> ThisActorInterface = CursorTarget;
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

void AAuraPlayerController::OnAbilityInputPressed(FGameplayTag InputTag)
{
	HoldDuration = 0.f;
	if (InputTag.MatchesTagExact(FAuraGameplayTagsManager::Get().InputTag_RMB))
	{
		return;
	}

	if (auto ASC = GetAuraAbilitySystemComponent())
	{
		ASC->OnAbilityInputPressed(InputTag);
	}
}

void AAuraPlayerController::OnAbilityInputHeld(FGameplayTag InputTag)
{
	HoldDuration += GetWorld()->GetDeltaSeconds();

	if (InputTag.MatchesTagExact(FAuraGameplayTagsManager::Get().InputTag_RMB))
	{
		if (!GetPawn())
		{
			return;
		}
		CachedMoveDestination = CursorHitResult.ImpactPoint;
		auto MoveLine = (CachedMoveDestination - GetPawn()->GetActorLocation());
		if (MoveLine.Size2D() < FinishMoveRadius)
		{
			return;
		}
		GetPawn()->AddMovementInput(MoveLine.GetSafeNormal2D());
		return;
	}

	if (auto ASC = GetAuraAbilitySystemComponent())
	{
		ASC->OnAbilityInputHeld(InputTag);
	}
}

void AAuraPlayerController::OnAbilityInputReleased(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTagsManager::Get().InputTag_RMB))
	{
		if (HoldDuration > ShortClickTime)
		{
			return;
		}
		if (!GetPawn())
		{
			return;
		}
		if ((CachedMoveDestination - GetPawn()->GetActorLocation()).Size2D() < FinishMoveRadius)
		{
			return;
		}

		auto NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetPawn()->GetActorLocation(), CachedMoveDestination);
		if (!NavPath)
		{
			return;
		}

		bIsNeedAutoMove = true;
		MoveSplineComponent->ClearSplinePoints();
		for (const auto& Point : NavPath->PathPoints)
		{
			MoveSplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::World, false);
		}
		MoveSplineComponent->UpdateSpline();
		AutoMoveIndex = 0;
	}

	if (auto ASC = GetAuraAbilitySystemComponent())
	{
		ASC->OnAbilityInputReleased(InputTag);
	}
}

TObjectPtr<UAuraAbilitySystemComponent> AAuraPlayerController::GetAuraAbilitySystemComponent()
{
	if (!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPlayerState<AAuraPlayerState>()));
	}
	return AuraAbilitySystemComponent;
}
