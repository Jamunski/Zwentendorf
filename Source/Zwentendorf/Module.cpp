// Fill out your copyright notice in the Description page of Project Settings.

#include "Module.h"

#include "Soul.h"

#include "Components/StaticMeshComponent.h"

AModule::AModule()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetEnableGravity(true);
}

void AModule::LinkSoul(ASoul *soul)
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
		UE_LOG(LogActor, Error, TEXT("Module not attached to a Soul"));
		HealthPoints = -1.0f;
	}

	return HealthPoints;
}
