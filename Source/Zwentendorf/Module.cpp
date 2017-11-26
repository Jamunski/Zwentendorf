// Fill out your copyright notice in the Description page of Project Settings.

#include "Module.h"

#include "Soul.h"

#include "Components/StaticMeshComponent.h"

AModule::AModule()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = MeshComponent;
}

void AModule::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UE_LOG(LogActor, Warning, TEXT("PostEditChangeProperty"));
}

void AModule::LinkSoul(APawn *soul)
{
	m_Soul = soul;
}

float AModule::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float HealthPoints = 0;

	if (m_Soul != NULL)
	{
		HealthPoints = m_Soul->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		//JV-TODO: Log error msg;
		HealthPoints = -1.0f;
	}

	return HealthPoints;
}
