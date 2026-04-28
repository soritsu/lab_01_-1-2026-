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
    float tiempoAcumulado;
    float intervalo;
    int indicemuro;
    int numero ;
    
    

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    TArray<AActor*> enemigos;
    UPROPERTY()
    TArray<AActor*> muros;
  

    
};