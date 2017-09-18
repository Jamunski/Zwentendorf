// Fill out your copyright notice in the Description page of Project Settings.

#include "MobilityDefault.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Components/StaticMeshComponent.h"

AMobilityDefault::AMobilityDefault()
{
	if (MeshComponent == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("AMobilityDefault"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/TwinStick/Meshes/Tasis/MOB_Default.MOB_Default"));
		// Create the mesh component
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		RootComponent = MeshComponent;
		MeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
		MeshComponent->SetStaticMesh(Mesh.Object);
	}
}

void AMobilityDefault::BeginPlay()
{
	Super::BeginPlay();

}
