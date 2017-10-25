// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"

#include "CoreMinimal.h"
#include "Module.h"
#include "ChassisModule.generated.h"

/**
 * 
 */
UCLASS(abstract)
class ZWENTENDORF_API AChassisModule : public AModule
{
	GENERATED_BODY()
	
public:
	/* The maximum health points of the robot*/
	UPROPERTY(Category = Health, EditAnywhere, BlueprintReadWrite)
		float MaximumHealthPoints;

	/* The current health points of the robot*/
	UPROPERTY(Category = Health, EditAnywhere, BlueprintReadWrite)
		float HealthPoints;

public:
	AChassisModule();
};
