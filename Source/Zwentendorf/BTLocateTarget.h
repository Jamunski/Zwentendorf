// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTLocateTarget.generated.h"

/**
 * 
 */
UCLASS()
class ZWENTENDORF_API UBTLocateTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
