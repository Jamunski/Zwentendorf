// Fill out your copyright notice in the Description page of Project Settings.

#include "BTEngage.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "SoulAIController.h"

EBTNodeResult::Type UBTEngage::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASoulAIController* AICon = Cast<ASoulAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		//Get target actor from blackboard, this will return null if BTSetTarget is not called or returns null
		UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComp();
		AActor *Target = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));

		if (Target)
		{
			AICon->ExecuteStrategy(EStrategyType::ENGAGE);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			UE_LOG(LogActor, Error, TEXT("UBTEngage::ExecuteTask - Target Actor not set, check if behaviour tree calls locateTarget before engage"));
		}
	}

	return EBTNodeResult::Failed;
}
