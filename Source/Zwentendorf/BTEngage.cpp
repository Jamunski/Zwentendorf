// Fill out your copyright notice in the Description page of Project Settings.

#include "BTEngage.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "BobAIController.h"

EBTNodeResult::Type UBTEngage::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//JV-TODO: Get Engagement Strategy from the soul??? This will allow for a generic BT task for any AI to extend

	ABobAIController* AICon = Cast<ABobAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		//Get target actor from blackboard, this will return null if BTSetTarget is not called or returns null
		UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComp();
		AActor *Target = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));

		if (Target)
		{
			AICon->ExecuteStrategy(EStrategyType::ENGAGE);
		}
	}

	return EBTNodeResult::Failed;
}
