#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
    TArray<AActor*> naves;
    TArray<AActor*> muros;
};