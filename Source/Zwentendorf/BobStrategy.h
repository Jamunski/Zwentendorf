// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strategy.h"
#include "BobStrategy.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ZWENTENDORF_API UBobStrategy : public UStrategy
{
	GENERATED_BODY()
	
public:
	UBobStrategy();

	virtual const bool ExecuteStrategy(ASoulAIController *soulAIController) override;
};
