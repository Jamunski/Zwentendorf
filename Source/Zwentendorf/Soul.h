// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Strategy.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Soul.generated.h"

class UBehaviorTree;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EStrategyType : uint8
{
	UNKNOWN			UMETA(DisplayName = "UNKNOWN"),
	ENGAGE			UMETA(DisplayName = "ENGAGE"), //Attacking a target, IE: Kamikaze
	LOCATE_TARGET	UMETA(DisplayName = "LOCATE_TARGET"), //How to find target, IE: Patrol
	SURVIVAL		UMETA(DisplayName = "SURVIVAL") //In danger, IE: Look for healing
};

UCLASS(abstract)
class ZWENTENDORF_API ASoul : public APawn
{
	GENERATED_BODY()

public:
	ASoul();

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual float ApplyDamage(const float damage) PURE_VIRTUAL(ASoul::ApplyDamage, return 0.0f; );

	virtual const float GetHealthPoints() PURE_VIRTUAL(ASoul::GetHealthPoints, return 0.0f; );

	virtual bool AttemptEnergyConsumption(const float amount) { return false; };

	virtual bool ExecuteStrategy(EStrategyType strategyType);

	virtual void SetMCUpdatedComponent(USceneComponent* NewUpdatedComponent);

	virtual float GetTotalMass() { return MeshComponent->GetMass(); };

	//Soul actions
	virtual void CalculateAimInput(float DeltaSeconds, FVector aimVector) {};

	virtual void LeftShoulder() {};
	virtual void RightShoulder() {};
	virtual void LeftTrigger() {};
	virtual void RightTrigger() {};

	virtual void Interact() {};
	virtual void Evade() {};
	virtual void AbilityX() {};
	virtual void AbilityY() {};

	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }

	UPROPERTY(EditAnywhere, Category = "AI")
		UBehaviorTree* BehaviorTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshComponent;

protected:
	void HandleDeath();

	/*
	brief implement to add custom death logic
	*/
	virtual void OnDeath() {};

	UPROPERTY(EditAnywhere, Category = "AI")
		TMap<EStrategyType, TSubclassOf<UStrategy> > StrategyMap;
};
