// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChassisModule.h"
#include "MobilityModule.h"
#include "WeaponModule.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tasis.generated.h"

UCLASS(Blueprintable)
class ATasis : public APawn
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	/** The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	//Modules
	UPROPERTY(Category = Module, EditAnywhere)
		AMobilityModule* m_Mobility;
	UPROPERTY(Category = Module, EditAnywhere)
		AChassisModule* m_Chassis;
	UPROPERTY(Category = Module, EditAnywhere)
		AWeaponModule* m_WeaponLeft;
	UPROPERTY(Category = Module, EditAnywhere)
		AWeaponModule* m_WeaponRight;


public:
	ATasis();

	// Begin Actor Interface
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

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

private:
	void SetupActionInput(UInputComponent* PlayerInputComponent);
	void SetupAxisInput(UInputComponent* PlayerInputComponent);
	void SetupAxisInputKeyboard(UInputComponent* PlayerInputComponent);
	void SetupAxisInputGamepad(UInputComponent* PlayerInputComponent);

	void MoveForward(float Value);
	void MoveRight(float Value);

	/* Aim in the specified direction */
	void CalculateAimInput();

	void LeftShoulder();
	void RightShoulder();
	void LeftTrigger() {};
	void RightTrigger() {};

	void Interact() {};
	void Dodge() {};
	void AbilityX() {};
	void AbilityY() {};

public:
	/** Returns Core MeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }

	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

