// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChassisModule.h"
#include "MobilityModule.h"
#include "Soul.h"
#include "WeaponModule.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tasis.generated.h"

UCLASS(Blueprintable)
class ATasis : public ASoul
{
	GENERATED_BODY()

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

	UPROPERTY(Category = Module, EditAnywhere)
		TSubclassOf<AMobilityModule> m_MobilityClass;
	UPROPERTY(Category = Module, EditAnywhere)
		TSubclassOf<AChassisModule> m_ChassisClass;
	UPROPERTY(Category = Module, EditAnywhere)
		TSubclassOf<AWeaponModule> m_WeaponLeftClass;
	UPROPERTY(Category = Module, EditAnywhere)
		TSubclassOf<AWeaponModule> m_WeaponRightClass;

public:
	ATasis();

	// Begin Actor Interface
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	// End Actor Interface

	// Begin Soul Interface
	virtual const float GetHealthPoints() override;
	virtual float ApplyDamage(const float damage) override;
	virtual bool AttemptEnergyConsumption(const float amount) override;

	virtual void OnDeath() override;

	virtual void SetMCUpdatedComponent(USceneComponent* NewUpdatedComponent) override;

	virtual float GetTotalMass() override;

	void CalculateAimInput(float DeltaSeconds, FVector aimVector) override;

	void LeftShoulder() override;
	void RightShoulder() override;
	void LeftTrigger() override {};
	void RightTrigger() override {};

	void Interact() override {};
	void Evade() override;
	void AbilityX() override {};
	void AbilityY() override {};
	// End Soul Interface

	FEnergyContainer GetEnergyContainer();

	void UpdateCoreColor();

public:
	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

