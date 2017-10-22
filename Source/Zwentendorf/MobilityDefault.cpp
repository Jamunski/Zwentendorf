// Fill out your copyright notice in the Description page of Project Settings.

#include "MobilityDefault.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "UObject/ConstructorHelpers.h"

AMobilityDefault::AMobilityDefault()
	: AMobilityModule()
{
	// Movement
	InitializeMesh();

	MoveSpeed = 1000.0f;
}

void AMobilityDefault::InitializeMesh()
{
	if (MeshComponent == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("AMobilityDefault"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Meshes/Character/Tasis/MOB_Default.MOB_Default"));
		// Create the mesh component
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		RootComponent = MeshComponent;
		MeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
		MeshComponent->SetStaticMesh(Mesh.Object);

		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetEnableGravity(true);
	}
}

void AMobilityDefault::BeginPlay()
{
	Super::BeginPlay();
}

void AMobilityDefault::CaclulateMovementInput(float DeltaSeconds, FVector movementVector)
{
	// Calculate  movement
	const FVector Movement = movementVector * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		FHitResult Hit(1.f);
		MeshComponent->MoveComponent(Movement, GetMeshComponent()->RelativeRotation, false);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
		}
	}
}
