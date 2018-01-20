// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"

#include "CoreMinimal.h"
#include "Module.h"
#include "ChassisModule.generated.h"

USTRUCT(BlueprintType)
struct FEnergyContainer
{
	GENERATED_BODY()

public:
	/* The maximum energy points of the robot*/
	UPROPERTY(Category = Energy, EditAnywhere, BlueprintReadWrite)
		float MaximumEnergyPool;

	/* The current energy points of the robot*/
	UPROPERTY(Category = Energy, EditAnywhere, BlueprintReadWrite)
		float EnergyPool;

	/* The rate in seconds at which energyPool recovers*/
	UPROPERTY(Category = Energy, EditAnywhere, BlueprintReadWrite)
		float EnergyRecoverySec;

	/* The maximum energyReserve points of the robot*/
	UPROPERTY(Category = Energy, EditAnywhere, BlueprintReadWrite)
		float MaximumEnergyReserve;

	/* The current energyReserve points of the robot*/
	UPROPERTY(Category = Energy, EditAnywhere, BlueprintReadWrite)
		float EnergyReserve;
};

/**
 * 
 */
UCLASS(abstract)
class ZWENTENDORF_API AChassisModule : public AModule
{
	GENERATED_BODY()
	
private:
	void EnergyDepletionTimerExpired();

	/** Handle for efficient management of RegenerateEnergyTimerExpired timer */
	FTimerHandle TimerHandle_EnergyDepletionRecoveryTimerExpired;

public:
	/* The maximum health points of the robot*/
	UPROPERTY(Category = Health, EditAnywhere, BlueprintReadWrite)
		float MaximumHealthPoints;

	/* The current health points of the robot*/
	UPROPERTY(Category = Health, EditAnywhere, BlueprintReadWrite)
		float HealthPoints;

	/* The EnergyContainer of the robot*/
	UPROPERTY(Category = Energy, EditAnywhere, BlueprintReadWrite)
		FEnergyContainer EnergyContainer;

public:
	AChassisModule();

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	// End Actor Interface

	virtual float ApplyDamage(const float damage);

	virtual FEnergyContainer DepleteEnergy(const float amount, bool &bSuccess);

	virtual FEnergyContainer RecoverEnergy(const float amount);

private:

	bool bRegenerateEnergy;
};
