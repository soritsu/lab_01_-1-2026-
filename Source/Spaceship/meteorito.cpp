#include "meteorito.h"

AMeteorito::AMeteorito()
{
  
    SetSpeed(400.0f);
    aceleracion = 150.0f;
}

void AMeteorito::CargarRuta()
{
    FVector inicio = GetActorLocation();

    ruta.Add(inicio);
    ruta.Add(inicio + FVector(0, 0, -2000.0f));
}

void AMeteorito::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    speed += aceleracion * DeltaTime;
}