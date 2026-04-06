#pragma once

#include "CoreMinimal.h"
#include "muro.h"
#include "muroMovil.generated.h"

UCLASS()
class SPACESHIP_API AMuroMovil : public AMuro
{
    GENERATED_BODY()

public:
    AMuroMovil();

    virtual void EfectoEspecial(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

private:
    FVector puntoA;
    FVector puntoB;
    float velocidad;
    bool yendo;
};