// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ZwentendorfGameMode.h"
#include "Tasis.h"
#include "SoulPlayerController.h"

#include <Runtime/Engine/Classes/Engine/LocalPlayer.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

AZwentendorfGameMode::AZwentendorfGameMode()
{
	NumberOfPlayers = 8;

	// set default pawn class to our character class
	DefaultPawnClass = ATasis::StaticClass();

	PlayerControllerClass = ASoulPlayerController::StaticClass();
}

void AZwentendorfGameMode::BeginPlay()
{
	Super::BeginPlay();

	// get how many players have already been created
	int createdPlayers = GetNumPlayers();

	// if the number of created players is less than the number of players we want for this game mode, create more...
	while (createdPlayers < NumberOfPlayers)
	{
		auto playerController = UGameplayStatics::CreatePlayer(GetWorld(), -1, true);
		createdPlayers++;
	}
}
