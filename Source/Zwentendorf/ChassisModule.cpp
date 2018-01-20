// Fill out your copyright notice in the Description page of Project Settings.

#include "ChassisModule.h"

#include "TimerManager.h"

#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>


static const float ENERGY_DEPLETION_RECOVERY_TIME = 0.5f;

AChassisModule::AChassisModule()
	: AModule()
	, bRegenerateEnergy(true)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true; //JV-TODO: set this to false by default, only chassis modules which are equipped by the player should tick...
	PrimaryActorTick.SetTickFunctionEnable(true);
}

void AChassisModule::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bRegenerateEnergy)
	{
		//JV-TODO: Update UI with the return of this call
		RecoverEnergy(EnergyContainer.EnergyRecoverySec * DeltaSeconds);
	}
}

float AChassisModule::ApplyDamage(const float damage)
{
	HealthPoints -= damage;

	return HealthPoints;
}

FEnergyContainer AChassisModule::DepleteEnergy(const float amount, bool &bSuccess)
{
	if (amount < EnergyContainer.EnergyPool + EnergyContainer.EnergyReserve)
	{
		float remainingAmount = amount > EnergyContainer.EnergyPool ? amount - EnergyContainer.EnergyPool : 0.0f;

		EnergyContainer.EnergyPool -= (amount - remainingAmount);
		EnergyContainer.EnergyReserve -= remainingAmount;

		bRegenerateEnergy = false;
		bSuccess = true;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_EnergyDepletionRecoveryTimerExpired, this, &AChassisModule::EnergyDepletionTimerExpired, ENERGY_DEPLETION_RECOVERY_TIME);
	}
	else
	{
		//JV-TODO: Indicate to the player that they do not have enough energy to perform this action

		bSuccess = false;
	}

	//JV-TODO: remove this after getting proper ui
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("pool: %f reserve: %f"), EnergyContainer.EnergyPool, EnergyContainer.EnergyReserve));

	return EnergyContainer;
}

void AChassisModule::EnergyDepletionTimerExpired()
{
	bRegenerateEnergy = true;

	//JV-TODO: indicate to the user that theire energy is recovering?
}

FEnergyContainer AChassisModule::RecoverEnergy(const float amount)
{
	EnergyContainer.EnergyPool = EnergyContainer.EnergyPool + amount < EnergyContainer.MaximumEnergyPool ? EnergyContainer.EnergyPool + amount : EnergyContainer.MaximumEnergyPool;

	return EnergyContainer;
}
