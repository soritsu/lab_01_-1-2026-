// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Spawner.h"
#include "SpaceshipGameMode.generated.h"

UCLASS(MinimalAPI)
class ASpaceshipGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASpaceshipGameMode();


protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY()
	ASpawner* EnemySpawner = nullptr;

	void InitSpawner();

};



