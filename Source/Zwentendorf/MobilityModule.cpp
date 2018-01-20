// Fill out your copyright notice in the Description page of Project Settings.

#include "MobilityModule.h"

#include "SoulMovementComponent.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


AMobilityModule::AMobilityModule()
	: AModule()
{
}

void AMobilityModule::Evade()
{
	USoulMovementComponent *MC = FindComponentByClass<USoulMovementComponent>();
	if (MC)
	{
		MC->Evade();
	}
}
