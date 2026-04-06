#pragma once

#include "CoreMinimal.h"
#include "muro.h"
#include "muroParpadeo.generated.h"

UCLASS()
class SPACESHIP_API AMuroParpadeo : public AMuro
{
    GENERATED_BODY()

public:
    AMuroParpadeo();

    virtual void EfectoEspecial(float DeltaTime) override;

private:
    float tiempoAcumulado;
    float intervalo;
    bool visible;
};