#include "meteorito.h"

AMeteorito::AMeteorito()
{
	
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
    
    if (MeshAsset.Succeeded())
    {
        meshEnemy->SetStaticMesh(MeshAsset.Object);
	}
    SetSpeed(400.0f);
    aceleracion = 50.0f;
}

void AMeteorito::CargarRuta()
{
    FVector inicio = GetActorLocation();

    ruta.Add(inicio);
    ruta.Add(inicio + FVector(0, 0, -1000.0f));
}

void AMeteorito::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    speed += aceleracion * DeltaTime;
}