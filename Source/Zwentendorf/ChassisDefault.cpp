// Fill out your copyright notice in the Description page of Project Settings.

#include "ChassisDefault.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "UObject/ConstructorHelpers.h"

AChassisDefault::AChassisDefault()
	: AChassisModule()
{
	//JV-TODO: Create a function in ChassisModule to do this sort of thing at some point
	//Initialize health points
	MaximumHealthPoints = 1000.0f;
	HealthPoints = MaximumHealthPoints;
}

void AChassisDefault::BeginPlay()
{
	Super::BeginPlay();
}
