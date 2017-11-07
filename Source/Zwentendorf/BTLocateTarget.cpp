// Fill out your copyright notice in the Description page of Project Settings.

#include "BTLocateTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

#include "SoulAIController.h"

EBTNodeResult::Type UBTLocateTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASoulAIController* AICon = Cast<ASoulAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		AICon->ExecuteStrategy(EStrategyType::LOCATE_TARGET);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
