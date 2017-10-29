// Fill out your copyright notice in the Description page of Project Settings.

#include "Bob.h"

#include "GameFramework/FloatingPawnMovement.h"

#include "Components/StaticMeshComponent.h"
#include "AIController.h"
#include "UObject/ConstructorHelpers.h"

ABob::ABob()
{
	// Bob Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Meshes/Character/Enemy/Bob.Bob"));
	// Create the mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
	MeshComponent->SetStaticMesh(Mesh.Object);

	UFloatingPawnMovement *movementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

	MaximumHealthPoints = 250.0f;
	HealthPoints = MaximumHealthPoints;
}

const float ABob::GetHealthPoints()
{
	return HealthPoints;
}

float ABob::ApplyDamage(const float damage)
{
	HealthPoints -= damage;

	return HealthPoints;
}

void ABob::OnDeath()
{
	//Explode or something?
	Destroy();
}
