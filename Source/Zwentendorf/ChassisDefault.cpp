// Fill out your copyright notice in the Description page of Project Settings.

#include "ChassisDefault.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "UObject/ConstructorHelpers.h"

AChassisDefault::AChassisDefault()
	: AChassisModule()
{
	//Initialize health points
	MaximumHealthPoints = 1000.0f;
	HealthPoints = MaximumHealthPoints;

	//Initialize energyPool points
	EnergyContainer.MaximumEnergyPool = 100.0f;
	EnergyContainer.EnergyPool = EnergyContainer.MaximumEnergyPool;

	EnergyContainer.EnergyRecoverySec = 40.0f;

	//Initialize energyReserve points
	EnergyContainer.MaximumEnergyReserve = 1000.0f;
	EnergyContainer.EnergyReserve = EnergyContainer.MaximumEnergyReserve;
}

void AChassisDefault::BeginPlay()
{
	Super::BeginPlay();
}
