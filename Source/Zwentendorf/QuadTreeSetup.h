// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuadTreeSetup.generated.h"

/**
 * 
 */
UCLASS()
class ZWENTENDORF_API UQuadTreeSetup : public UObject
{
	GENERATED_BODY()
	
public:

	void Setup(FRandomStream randomStream);

private:
	FVector MapDimensions;
};
