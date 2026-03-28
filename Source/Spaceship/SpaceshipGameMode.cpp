// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceshipGameMode.h"
#include "SpaceshipPawn.h"
#include "enemigo.h"


ASpaceshipGameMode::ASpaceshipGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ASpaceshipPawn::StaticClass();
}
void ASpaceshipGameMode::BeginPlay()
{
	Super::BeginPlay();
	FVector SpawnLocation = FVector(0.0f, 0.0f, 160.0f);

	// Spawn the enemy actor
	GetWorld()->SpawnActor<Aenemigo>(Aenemigo::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
}

