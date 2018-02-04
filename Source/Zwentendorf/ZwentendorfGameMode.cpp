// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ZwentendorfGameMode.h"
#include "Tasis.h"
#include "SoulPlayerController.h"

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

AZwentendorfGameMode::AZwentendorfGameMode()
{
	NumberOfPlayers = 1;

	// set default pawn class to our character class
	DefaultPawnClass = ATasis::StaticClass();

	PlayerControllerClass = ASoulPlayerController::StaticClass();
}

void AZwentendorfGameMode::BeginPlay()
{
	Super::BeginPlay();

	//JV-TODO: Get the number of players who want to play the game and create a player object for each of them
	
	int createdPlayers = 1;

	while (createdPlayers < NumberOfPlayers)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), -1, true);
		createdPlayers++;
	}
}
