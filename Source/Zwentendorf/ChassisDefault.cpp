// Fill out your copyright notice in the Description page of Project Settings.

#include "ChassisDefault.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Components/StaticMeshComponent.h"


AChassisDefault::AChassisDefault()
{
	if (MeshComponent == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("AChassisDefault"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/TwinStick/Meshes/Tasis/CHA_Default.CHA_Default"));
		// Create the mesh component
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		RootComponent = MeshComponent;
		MeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
		MeshComponent->SetStaticMesh(Mesh.Object);
	}
}

void AChassisDefault::BeginPlay()
{
	Super::BeginPlay();

}
