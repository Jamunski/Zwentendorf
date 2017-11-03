// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ZwentendorfGameMode.h"
#include "Tasis.h"
#include "SoulPlayerController.h"

AZwentendorfGameMode::AZwentendorfGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ATasis::StaticClass();

	PlayerControllerClass = ASoulPlayerController::StaticClass();
}

