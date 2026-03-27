// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceshipGameMode.h"
#include "SpaceshipPawn.h"

ASpaceshipGameMode::ASpaceshipGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ASpaceshipPawn::StaticClass();
}

