// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Strategy.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Soul.generated.h"

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
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	/* The mesh component */
	UPROPERTY(Category = "Mesh", EditAnywhere)
		class UStaticMeshComponent* MeshComponent;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TMap<EStrategyType, UStrategy*> StrategyMap;

public:
	ASoul();

	//CALL IN DERIVED
	virtual void InitializeSoul();

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual const float GetHealthPoints() PURE_VIRTUAL(ASoul::GetHealthPoints, return 0.0f ;);

	virtual float ApplyDamage(const float damage) PURE_VIRTUAL(ASoul::GetHealthPoints, return 0.0f ;);

	virtual bool ExecuteStrategy(EStrategyType strategy);

protected:
	void HandleDeath();

	/*
	brief implement to add custom death logic
	*/
	virtual void OnDeath() {};

public:
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }
};
