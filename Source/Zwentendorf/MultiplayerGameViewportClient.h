// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "MultiplayerGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class ZWENTENDORF_API UMultiplayerGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	UMultiplayerGameViewportClient(const FObjectInitializer &ObjectInitializer);
	
	virtual void UpdateActiveSplitscreenType() override;

	virtual void SetDisableSplitscreenOverrideHack(const bool bDisabled);

protected:
	virtual void LayoutPlayersHack();

	bool bDisableSplitScreenOverrideHack;
};
