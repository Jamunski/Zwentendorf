// Fill out your copyright notice in the Description page of Project Settings.

#include "MobilityDefault.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "UObject/ConstructorHelpers.h"

AMobilityDefault::AMobilityDefault()
	: AMobilityModule()
{
}

void AMobilityDefault::BeginPlay()
{
	Super::BeginPlay();
}
