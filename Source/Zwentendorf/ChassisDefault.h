// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChassisModule.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChassisDefault.generated.h"

UCLASS()
class ZWENTENDORF_API AChassisDefault : public AChassisModule
{
	GENERATED_BODY()

public:
	AChassisDefault();

protected:
	virtual void BeginPlay() override;

	virtual void InitializeMesh() override;
};
