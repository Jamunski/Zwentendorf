// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MobilityModule.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MobilityDefault.generated.h"

UCLASS()
class ZWENTENDORF_API AMobilityDefault : public AMobilityModule
{
	GENERATED_BODY()

public:
	AMobilityDefault();

protected:
	virtual void BeginPlay() override;
};

