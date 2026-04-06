#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "muro.generated.h"

UCLASS()
class SPACESHIP_API AMuro : public AActor
{
    GENERATED_BODY()

public:
    AMuro();
    virtual void Tick(float DeltaTime) override;

    virtual void EfectoEspecial(float DeltaTime);

    float GetVida() const { return vida; }
    void SetVida(float newVida) { vida = newVida; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* meshMuro;

    float vida;
};