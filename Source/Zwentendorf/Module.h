// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Module.generated.h"

UCLASS(abstract)
class ZWENTENDORF_API AModule : public AActor
{
	GENERATED_BODY()

protected:
	/* The mesh component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* MeshComponent;

	UPROPERTY(Category = Soul, EditAnywhere)
		APawn* m_Soul; //JV-TODO: Use ASoul class instead of APawn 

public:	
	AModule();

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void LinkSoul(APawn *soul); //JV-TODO: Use ASoul class instead of APawn 

	/** Returns MobilityMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
