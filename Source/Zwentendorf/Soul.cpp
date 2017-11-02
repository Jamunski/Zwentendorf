// Fill out your copyright notice in the Description page of Project Settings.

#include "Soul.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ASoul::ASoul()
{
}

void ASoul::InitializeSoul()
{
	// Bob Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Meshes/Character/Enemy/Bob.Bob"));
	// Create the mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
	MeshComponent->SetStaticMesh(Mesh.Object);
}

float ASoul::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float healthPoints = GetHealthPoints();

	if (DamageAmount > 0.001f)
	{
		healthPoints = ApplyDamage(DamageAmount);

		if (healthPoints <= 0.0f)
		{
			HandleDeath();
		}
	}

	return healthPoints;
}

void ASoul::HandleDeath()
{
	//Logic goes here...

	OnDeath();
}

bool ASoul::ExecuteStrategy(EStrategyType strategy)
{
	return StrategyMap[strategy]->ExecuteStrategy();
}
