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
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName AimForwardBinding;
	static const FName AimRightBinding;

	// Static names for action bindings
	static const FName LeftShoulderBinding;
	static const FName RightShoulderBinding;
	static const FName LeftTriggerBinding;
	static const FName RightTriggerBinding;
	static const FName InteractBinding;
	static const FName DodgeBinding;
	static const FName AbilityXBinding;
	static const FName AbilityYBinding;

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

	void LeftShoulder();
	void RightShoulder();
	void LeftTrigger();
	void RightTrigger();

	void Interact();
	void Evade();
	void AbilityX();
	void AbilityY();
	//Soul actions end

	bool bUsingGamepad;
};
