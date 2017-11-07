// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strategy.h"
#include "OmniscientLocateTargetStrategy.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ZWENTENDORF_API UOmniscientLocateTargetStrategy : public UStrategy
{
	GENERATED_BODY()
	
public:
	UOmniscientLocateTargetStrategy();

	virtual const bool ExecuteStrategy(ASoulAIController *soulAIController) override;
};
