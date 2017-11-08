// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicSoul.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UObject/ConstructorHelpers.h"


ABasicSoul::ABasicSoul()
	: ASoul()
{
}

const float ABasicSoul::GetHealthPoints()
{
	return HealthPoints;
}

float ABasicSoul::ApplyDamage(const float damage)
{
	HealthPoints -= damage;

	return HealthPoints;
}

void ABasicSoul::OnDeath()
{
	Destroy();
}
