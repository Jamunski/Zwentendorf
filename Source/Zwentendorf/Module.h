// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Module.generated.h"

class ASoul;

UCLASS(abstract)
class ZWENTENDORF_API AModule : public AActor
{
	GENERATED_BODY()

public:	
	AModule();

	virtual void LinkSoul(ASoul *soul);

	/** Returns MobilityMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* MeshComponent;
	UPROPERTY(Category = Soul, BlueprintReadOnly)
		ASoul* m_Soul;
	/** Returns the soul that this module is attached to. Returns null if this module is not attached to a soul**/
	UFUNCTION(BlueprintCallable, Category = "Soul")
		virtual ASoul *GetAttachedSoul();
};
