#pragma once

#include "CoreMinimal.h"
#include "enemigo.h"
#include "alien.generated.h"

UCLASS()
class SPACESHIP_API AAlien : public Aenemigo
{
    GENERATED_BODY()

public:
    AAlien();

    virtual void CargarRuta() override;
};