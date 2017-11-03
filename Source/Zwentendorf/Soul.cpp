// Fill out your copyright notice in the Description page of Project Settings.

#include "Soul.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ASoul::ASoul()
{
	InitializeSoul();
}

void ASoul::InitializeSoul()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void ASoul::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UE_LOG(LogActor, Warning, TEXT("PostEditChangeProperty"));
	if (Mesh)
	{
		UE_LOG(LogActor, Warning, TEXT("Mesh Valid"));
		MeshComponent->SetStaticMesh(Mesh);
		RootComponent = MeshComponent;
	}
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
	OnDeath();
}

bool ASoul::ExecuteStrategy(EStrategyType strategy)
{
	return StrategyMap[strategy]->ExecuteStrategy();
}
