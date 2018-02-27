// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulPlayerController.h"

#include "ZwentendorfGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"

#include <Runtime/Engine/Classes/Engine/LocalPlayer.h>
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

const FName ASoulPlayerController::Binding_AnyKeyButton("Any_KeyButton");

const FName ASoulPlayerController::Binding_MoveForward("MoveForward");
const FName ASoulPlayerController::Binding_MoveRight("MoveRight");
const FName ASoulPlayerController::Binding_AimForward("AimForward");
const FName ASoulPlayerController::Binding_AimRight("AimRight");

const FName ASoulPlayerController::Binding_ChassisOne("ChassisSlot_One");
const FName ASoulPlayerController::Binding_ChassisTwo("ChassisSlot_Two");
const FName ASoulPlayerController::Binding_ChassisThree("ChassisSlot_Three");
const FName ASoulPlayerController::Binding_ChassisFour("ChassisSlot_Four");
const FName ASoulPlayerController::Binding_ChassisFive("ChassisSlot_Five");
const FName ASoulPlayerController::Binding_ChassisSix("ChassisSlot_Six");
const FName ASoulPlayerController::Binding_Interact("Interact");
const FName ASoulPlayerController::Binding_Evade("Evade");

const FName ASoulPlayerController::Binding_Pause("Pause");

void ASoulPlayerController::SetWaitingForInputFunctor(UWaitingForInputFunctor* functor)
{
	Functor_WaitingForInput = functor;
}

ASoulPlayerController::ASoulPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bUsingGamepad = false;
	bWaitingForInput = true;

	bShowMouseCursor = true;
}

void ASoulPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PossessedSoul)
	{
		FVector movementVector{ GetInputAxisValue(Binding_MoveForward), GetInputAxisValue(Binding_MoveRight), 0.0f };
		PossessedSoul->AddMovementInput(movementVector);

		FVector aimVector{ GetInputAxisValue(Binding_AimForward), GetInputAxisValue(Binding_AimRight), 0.0f };
		CalculateAimInput(DeltaSeconds, aimVector);
	}
}

void ASoulPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	if (bWaitingForInput)
	{
		SetupWaitingForInput(true);
	}
	else
	{
		BindGameplayInput();
	}
}

void ASoulPlayerController::BindGameplayInput()
{
	UE_LOG(LogInput, Log, TEXT("BindGameplayInput: %s, ControllerID: %d"), *GetName(), GetLocalPlayer()->GetControllerId());

	SetupActionInput();
	SetupAxisInput();
	SetupAxisInputKeyboard();
	SetupAxisInputGamepad();
}

void ASoulPlayerController::UnbindGameplayInput(bool bShouldWaitForInput)
{
	UE_LOG(LogInput, Log, TEXT("UnbindGameplayInput $s, ControllerID: %d"), *GetName(), GetLocalPlayer()->GetControllerId());

	InputComponent->ClearBindingValues();
	InputComponent->ClearActionBindings();

	SetupWaitingForInput(bShouldWaitForInput);
}

void ASoulPlayerController::SetupWaitingForInput(bool bShouldWaitForInput)
{
	bWaitingForInput = bShouldWaitForInput;

	if (bWaitingForInput)
	{
		InputComponent->BindAction(Binding_AnyKeyButton, IE_Pressed, this, &ASoulPlayerController::Any_Pressed);
		InputComponent->BindAction(Binding_AnyKeyButton, IE_Released, this, &ASoulPlayerController::Any_Released);
	}
}

void ASoulPlayerController::SetupActionInput()
{
	InputComponent->BindAction(Binding_ChassisOne, IE_Pressed, this, &ASoulPlayerController::ChassisSlot_One_Pressed);
	InputComponent->BindAction(Binding_ChassisOne, IE_Released, this, &ASoulPlayerController::ChassisSlot_One_Released);

	InputComponent->BindAction(Binding_ChassisTwo, IE_Pressed, this, &ASoulPlayerController::ChassisSlot_Two_Pressed);
	InputComponent->BindAction(Binding_ChassisTwo, IE_Released, this, &ASoulPlayerController::ChassisSlot_Two_Released);

	InputComponent->BindAction(Binding_ChassisThree, IE_Pressed, this, &ASoulPlayerController::ChassisSlot_Three_Pressed);
	InputComponent->BindAction(Binding_ChassisThree, IE_Released, this, &ASoulPlayerController::ChassisSlot_Three_Released);

	InputComponent->BindAction(Binding_ChassisFour, IE_Pressed, this, &ASoulPlayerController::ChassisSlot_Four_Pressed);
	InputComponent->BindAction(Binding_ChassisFour, IE_Released, this, &ASoulPlayerController::ChassisSlot_Four_Released);

	InputComponent->BindAction(Binding_ChassisFive, IE_Pressed, this, &ASoulPlayerController::ChassisSlot_Five_Pressed);
	InputComponent->BindAction(Binding_ChassisFive, IE_Released, this, &ASoulPlayerController::ChassisSlot_Five_Released);

	InputComponent->BindAction(Binding_ChassisSix, IE_Pressed, this, &ASoulPlayerController::ChassisSlot_Six_Pressed);
	InputComponent->BindAction(Binding_ChassisSix, IE_Released, this, &ASoulPlayerController::ChassisSlot_Six_Released);

	InputComponent->BindAction(Binding_Interact, IE_Pressed, this, &ASoulPlayerController::Interact_Pressed);
	InputComponent->BindAction(Binding_Interact, IE_Released, this, &ASoulPlayerController::Interact_Released);

	InputComponent->BindAction(Binding_Evade, IE_Pressed, this, &ASoulPlayerController::Evade_Pressed);
	InputComponent->BindAction(Binding_Evade, IE_Released, this, &ASoulPlayerController::Evade_Released);

	InputComponent->BindAction(Binding_Pause, IE_Pressed, this, &ASoulPlayerController::Pause_Pressed);
	InputComponent->BindAction(Binding_Pause, IE_Released, this, &ASoulPlayerController::Pause_Released);
}

void ASoulPlayerController::SetupAxisInput()
{
	InputComponent->BindAxis(Binding_MoveForward);
	InputComponent->BindAxis(Binding_MoveRight);

	InputComponent->BindAxis(Binding_AimForward);
	InputComponent->BindAxis(Binding_AimRight);
}

void ASoulPlayerController::SetupAxisInputKeyboard()
{
}

void ASoulPlayerController::SetupAxisInputGamepad()
{
}

void ASoulPlayerController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	GetLocalPlayer()->GetControllerId() == 0 ? bUsingGamepad = false : bUsingGamepad = true;

	PossessedSoul = Cast<ASoul>(Pawn);
}

void ASoulPlayerController::UnPossess()
{
	Super::UnPossess();

	PossessedSoul = nullptr;
}

ASoul * ASoulPlayerController::GetPossessedSoul()
{
	return PossessedSoul;
}

/*_______________________ Soul Actions _______________________*/
void ASoulPlayerController::OnInputReceivedWhileWaiting()
{
	if (Functor_WaitingForInput)
	{
		Functor_WaitingForInput->operator()(this);
	}
}

void ASoulPlayerController::CalculateAimInput(float DeltaSeconds, FVector aimVector) { bUsingGamepad ? CalculateGamepadAimInput(DeltaSeconds, aimVector) : CalculateMouseAimInput(DeltaSeconds, aimVector); }

void ASoulPlayerController::CalculateGamepadAimInput(float DeltaSeconds, FVector aimVector)
{
	if (PossessedSoul)
	{
		PossessedSoul->CalculateAimInput(DeltaSeconds, aimVector);
	}
}

void ASoulPlayerController::CalculateMouseAimInput(float DeltaSeconds, FVector aimVector)
{
	if (PossessedSoul)
	{
		FVector mouseLocation;
		FVector worldDirection;

		FVector actorLocation = PossessedSoul->GetActorLocation();

		ULocalPlayer* const LocalPlayer = GetLocalPlayer();
		if (LocalPlayer && LocalPlayer->ViewportClient)
		{
			FVector2D MousePosition = LocalPlayer->ViewportClient->GetMousePosition();
			FVector2D ScreenPosition = FVector2D::ZeroVector;

			FVector actorLocation = PossessedSoul->GetActorLocation();

			if (ProjectWorldLocationToScreen(actorLocation, ScreenPosition))
			{
				FVector dirVec = FVector::ZeroVector;
				dirVec.Y = (MousePosition.X - ScreenPosition.X);
				dirVec.X = -(MousePosition.Y - ScreenPosition.Y);

				PossessedSoul->CalculateAimInput(DeltaSeconds, dirVec);
			}
		}
	}
}

//JV-TODO: Instead of early return, maybe we should unbind gameplay actions and enable bWaitingForInput. Once input is received we should check if the input was handled and then rebind gameplay actions.

void ASoulPlayerController::Any_Pressed() { if (bWaitingForInput) { OnInputReceivedWhileWaiting(); } }

void ASoulPlayerController::ChassisSlot_One_Pressed() { if (PossessedSoul) { PossessedSoul->ChassisSlot_One_Pressed(); } }

void ASoulPlayerController::ChassisSlot_Two_Pressed() { if (PossessedSoul) { PossessedSoul->ChassisSlot_Two_Pressed(); } }

void ASoulPlayerController::ChassisSlot_Three_Pressed() { if (PossessedSoul) { PossessedSoul->ChassisSlot_Three_Pressed(); } }

void ASoulPlayerController::ChassisSlot_Four_Pressed() { if (PossessedSoul) { PossessedSoul->ChassisSlot_Four_Pressed(); } }

void ASoulPlayerController::ChassisSlot_Five_Pressed() { if (PossessedSoul) { PossessedSoul->ChassisSlot_Five_Pressed(); } }

void ASoulPlayerController::ChassisSlot_Six_Pressed() { if (PossessedSoul) { PossessedSoul->ChassisSlot_Six_Pressed(); } }

void ASoulPlayerController::Interact_Pressed() { if (PossessedSoul) { PossessedSoul->Interact_Pressed(); } }

void ASoulPlayerController::Evade_Pressed() { if (PossessedSoul) { PossessedSoul->Evade_Pressed(); } }

void ASoulPlayerController::Pause_Pressed() { /*JV-TODO: Pause the game*/ UE_LOG(LogInput, Log, TEXT("Input - Pause_Pressed")); }
