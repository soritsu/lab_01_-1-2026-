#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceshipGameMode.generated.h"

UCLASS(MinimalAPI)// El MinimalAPI es para que el editor pueda reconocer esta clase sin necesidad de exportarla completamente
class ASpaceshipGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASpaceshipGameMode();

protected:
    virtual void BeginPlay() override;

private:
    TArray<AActor*> enemigos;
    TArray<AActor*> muros;
};