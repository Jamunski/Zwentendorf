// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Components/StaticMeshComponent.h"


ACannon::ACannon()
{
	if (MeshComponent == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("ACannon"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/TwinStick/Meshes/Tasis/WEA_Cannon.WEA_Cannon"));
		// Create the mesh component
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		RootComponent = MeshComponent;
		MeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
		MeshComponent->SetStaticMesh(Mesh.Object);
	}
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
}
