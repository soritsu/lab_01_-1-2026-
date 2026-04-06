#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "enemigo.generated.h"

UCLASS()
class SPACESHIP_API Aenemigo : public AActor
{
    GENERATED_BODY()

public:
    Aenemigo();
    virtual void Tick(float DeltaTime) override;

    // Getters
    float GetSpeed() const { return speed; }
    int GetIndiceRuta() const { return indiceRuta; }

    // Setters
    void SetSpeed(float newSpeed) { speed = newSpeed; }

    virtual void CargarRuta();

protected:
    virtual void BeginPlay() override;
    void Mover(float DeltaTime);

    TArray<FVector> ruta;
    int indiceRuta;
    float speed;

public:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* meshEnemy;
};