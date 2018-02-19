// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Soul.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SoulPlayerController.generated.h"

/**
 *
 */
UCLASS()
class ZWENTENDORF_API ASoulPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASoulPlayerController();

	//Begin PlayerController interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	virtual void Possess(APawn* Pawn) override;
	//End PlayerController interface

	// Static names for axis bindings
	static const FName Binding_MoveForward;
	static const FName Binding_MoveRight;
	static const FName Binding_AimForward;
	static const FName Binding_AimRight;

	// Static names for action bindings
	static const FName Binding_ChassisOne;
	static const FName Binding_ChassisTwo;
	static const FName Binding_ChassisThree;
	static const FName Binding_ChassisFour;
	static const FName Binding_ChassisFive;
	static const FName Binding_ChassisSix;
	static const FName Binding_Interact;
	static const FName Binding_Evade;

	static const FName Binding_Pause;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Soul")
		ASoul *PossessedSoul;

private:
	//Input Setup
	void SetupActionInput();
	void SetupAxisInput();
	void SetupAxisInputKeyboard();
	void SetupAxisInputGamepad();
	//Input Setup end

	//Soul actions
	void CalculateAimInput(float DeltaSeconds, FVector aimVector);
	void CalculateGamepadAimInput(float DeltaSeconds, FVector aimVector);
	void CalculateMouseAimInput(float DeltaSeconds, FVector aimVector);

	void ChassisSlot_One_Pressed();
	void ChassisSlot_One_Released() {};
	void ChassisSlot_Two_Pressed();
	void ChassisSlot_Two_Released() {};
	void ChassisSlot_Three_Pressed();
	void ChassisSlot_Three_Released() {};
	void ChassisSlot_Four_Pressed();
	void ChassisSlot_Four_Released() {};
	void ChassisSlot_Five_Pressed();
	void ChassisSlot_Five_Released() {};
	void ChassisSlot_Six_Pressed();
	void ChassisSlot_Six_Released() {};

	void Interact_Pressed();
	void Interact_Released() {};
	void Evade_Pressed();
	void Evade_Released() {};

	void Pause_Pressed();
	void Pause_Released() {};
	
	//Soul actions end

	bool bUsingGamepad;
	bool bWaitingForInput;
};
