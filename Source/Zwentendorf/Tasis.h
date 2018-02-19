// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChassisModule.h"
#include "MobilityModule.h"
#include "WeaponModule.h"

#include "CoreMinimal.h"
#include "Soul.h"
#include "Tasis.generated.h"

UCLASS(Blueprintable)
class ATasis : public ASoul
{
	GENERATED_BODY()

public:
	ATasis();

	// Begin Actor Interface
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	// End Actor Interface

	virtual void OnDeath() override;
	virtual const float GetHealthPoints() override;
	virtual float ApplyDamage(const float damage) override;
	virtual void UpdateCoreColor();

	virtual bool AttemptEnergyConsumption(const float amount) override;
	FEnergyContainer GetEnergyContainer();

	virtual void SetMCUpdatedComponent(USceneComponent* NewUpdatedComponent) override;

	virtual float GetTotalMass() override;

	// Soul Actions
	void CalculateAimInput(float DeltaSeconds, FVector aimVector) override;

	void ChassisSlot_One_Pressed() override;
	void ChassisSlot_One_Released() {};
	void ChassisSlot_Two_Pressed() override;
	void ChassisSlot_Two_Released() {};
	void ChassisSlot_Three_Pressed() {};
	void ChassisSlot_Three_Released() {};
	void ChassisSlot_Four_Pressed() {};
	void ChassisSlot_Four_Released() {};
	void ChassisSlot_Five_Pressed() {};
	void ChassisSlot_Five_Released() {};
	void ChassisSlot_Six_Pressed() {};
	void ChassisSlot_Six_Released() {};

	void Interact_Pressed() {};
	void Interact_Released() {};
	void Evade_Pressed() override;
	void Evade_Released() {};

	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComponent;
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	//JV-TODO: review this...
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
};

