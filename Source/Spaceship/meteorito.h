#pragma once

#include "CoreMinimal.h"
#include "enemigo.h"
#include "meteorito.generated.h"

UCLASS()
class SPACESHIP_API AMeteorito : public Aenemigo
{
    GENERATED_BODY()

public:
    AMeteorito();

    virtual void Tick(float DeltaTime) override;
    virtual void CargarRuta() override;

private:
    float aceleracion;
};