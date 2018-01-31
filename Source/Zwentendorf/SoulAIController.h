// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Soul.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "SoulAIController.generated.h"

UCLASS()
class ZWENTENDORF_API ASoulAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASoulAIController();

	virtual void SetTarget(AActor *Target);

	void ExecuteStrategy(EStrategyType strategy);

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	FORCEINLINE TArray<AActor*> GetAvailableTargetPoints() { return BotTargetPoints; }

protected:
	virtual void Possess(APawn* Pawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Soul")
		ASoul *PossessedSoul;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		AActor *TargetActor;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName LocationToGoKey;

	TArray<AActor*> BotTargetPoints;
	UBehaviorTreeComponent* BehaviorComp;
	UBlackboardComponent* BlackboardComp;
};
