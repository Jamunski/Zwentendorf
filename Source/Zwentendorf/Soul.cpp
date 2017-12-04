// Fill out your copyright notice in the Description page of Project Settings.

#include "Soul.h"

#include "SoulAIController.h"
#include "SoulMovementComponent.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ASoul::ASoul()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
}

void ASoul::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UE_LOG(LogActor, Warning, TEXT("PostEditChangeProperty"));
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

bool ASoul::ExecuteStrategy(EStrategyType strategyType)
{
	if (GetController() && GetController()->IsA(ASoulAIController
		::StaticClass()))
	{
		ASoulAIController *aiController = Cast<ASoulAIController>(GetController());
		
		if (aiController && StrategyMap.Contains(strategyType))
		{
			UStrategy *strategy = StrategyMap[strategyType].GetDefaultObject();
			if (strategy)
			{
				//Pass the AI controller into the function...
				return strategy->ExecuteStrategy(aiController);
			}
		}
	}

	return false;
}

void ASoul::SetMCUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	USoulMovementComponent *MC = FindComponentByClass<USoulMovementComponent>();
	if (MC && RootComponent)
	{
		MC->SetUpdatedComponent(NewUpdatedComponent);
	}
}
