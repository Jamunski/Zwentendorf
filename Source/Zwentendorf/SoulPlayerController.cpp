// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulPlayerController.h"


const FName ASoulPlayerController::MoveForwardBinding("MoveForward");
const FName ASoulPlayerController::MoveRightBinding("MoveRight");
const FName ASoulPlayerController::AimForwardBinding("AimForward");
const FName ASoulPlayerController::AimRightBinding("AimRight");

const FName ASoulPlayerController::LeftShoulderBinding("Weapon-LeftShoulder");
const FName ASoulPlayerController::RightShoulderBinding("Weapon-RightShoulder");
const FName ASoulPlayerController::LeftTriggerBinding("Weapon-LeftTrigger");
const FName ASoulPlayerController::RightTriggerBinding("Weapon-RightTrigger");
const FName ASoulPlayerController::InteractBinding("Interact");
const FName ASoulPlayerController::DodgeBinding("Dodge");
const FName ASoulPlayerController::AbilityXBinding("Ability-X");
const FName ASoulPlayerController::AbilityYBinding("Ability-Y");

ASoulPlayerController::ASoulPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASoulPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PossessedSoul)
	{
		FVector movementVector{GetInputAxisValue(MoveForwardBinding), GetInputAxisValue(MoveRightBinding), 0.0f};
		//CaclulateMovementInput(DeltaSeconds, movementVector);
		PossessedSoul->AddMovementInput(movementVector);

		FVector aimVector{ GetInputAxisValue(AimForwardBinding), GetInputAxisValue(AimRightBinding), 0.0f };
		CalculateAimInput(DeltaSeconds, aimVector);
	}
}

void ASoulPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	SetupActionInput();
	SetupAxisInput();
	SetupAxisInputKeyboard();
	SetupAxisInputGamepad();
}

void ASoulPlayerController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	PossessedSoul = Cast<ASoul>(Pawn);
}

void ASoulPlayerController::SetupActionInput()
{
	//JV-TODO:
	// set up gameplay weapon bindings
	InputComponent->BindAction(LeftShoulderBinding, IE_Pressed, this, &ASoulPlayerController::LeftShoulder);
	//PlayerInputComponent->BindAction(InteractBinding, IE_Released, this, &ASoulPlayerController::InteractReleased);
	InputComponent->BindAction(RightShoulderBinding, IE_Pressed, this, &ASoulPlayerController::RightShoulder);
	//PlayerInputComponent->BindAction(InteractBinding, IE_Released, this, &ASoulPlayerController::InteractReleased);
	InputComponent->BindAction(LeftTriggerBinding, IE_Pressed, this, &ASoulPlayerController::LeftTrigger);
	//PlayerInputComponent->BindAction(InteractBinding, IE_Released, this, &ASoulPlayerController::InteractReleased);
	InputComponent->BindAction(RightTriggerBinding, IE_Pressed, this, &ASoulPlayerController::RightTrigger);
	//PlayerInputComponent->BindAction(InteractBinding, IE_Released, this, &ASoulPlayerController::InteractReleased);

	//// set up gameplay ability bindings
	InputComponent->BindAction(InteractBinding, IE_Pressed, this, &ASoulPlayerController::Interact);
	InputComponent->BindAction(DodgeBinding, IE_Pressed, this, &ASoulPlayerController::Evade);
	InputComponent->BindAction(AbilityXBinding, IE_Pressed, this, &ASoulPlayerController::AbilityX);
	InputComponent->BindAction(AbilityYBinding, IE_Pressed, this, &ASoulPlayerController::AbilityY);
}

void ASoulPlayerController::SetupAxisInput()
{
	InputComponent->BindAxis(MoveForwardBinding);
	InputComponent->BindAxis(MoveRightBinding);

	InputComponent->BindAxis(AimForwardBinding);
	InputComponent->BindAxis(AimRightBinding);
}

void ASoulPlayerController::SetupAxisInputKeyboard()
{
}

void ASoulPlayerController::SetupAxisInputGamepad()
{
}

/*_______________________ Soul Actioons _______________________*/
void ASoulPlayerController::CaclulateMovementInput(float DeltaSeconds, FVector movementVector)
{
	if (PossessedSoul)
	{
		PossessedSoul->CaclulateMovementInput(DeltaSeconds, movementVector);
	}
}

void ASoulPlayerController::CalculateAimInput(float DeltaSeconds, FVector aimVector)
{
	if (PossessedSoul)
	{
		PossessedSoul->CalculateAimInput(DeltaSeconds, aimVector);
	}
}

void ASoulPlayerController::LeftShoulder()
{
	if (PossessedSoul)
	{
		PossessedSoul->LeftShoulder();
	}
}

void ASoulPlayerController::RightShoulder()
{
	if (PossessedSoul)
	{
		PossessedSoul->RightShoulder();
	}
}

void ASoulPlayerController::LeftTrigger()
{
	if (PossessedSoul)
	{
		PossessedSoul->LeftTrigger();
	}
}

void ASoulPlayerController::RightTrigger()
{
	if (PossessedSoul)
	{
		PossessedSoul->RightTrigger();
	}
}

void ASoulPlayerController::Interact()
{
	if (PossessedSoul)
	{
		PossessedSoul->Interact();
	}
}

void ASoulPlayerController::Evade()
{
	UE_LOG(LogActor, Warning, TEXT("ASoulPlayerController::Evade"));
	if (PossessedSoul)
	{
		PossessedSoul->Evade();
	}
}

void ASoulPlayerController::AbilityX()
{
	if (PossessedSoul)
	{
		PossessedSoul->AbilityX();
	}
}

void ASoulPlayerController::AbilityY()
{
	if (PossessedSoul)
	{
		PossessedSoul->AbilityY();
	}
}
