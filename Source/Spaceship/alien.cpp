#include "alien.h"

AAlien::AAlien()
{
	// Cargar malla cono
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone"));
    if (MeshAsset.Succeeded())
    {
        meshEnemy->SetStaticMesh(MeshAsset.Object);
    }
    SetSpeed(800.0f);

}

void AAlien::CargarRuta()
{
    FVector inicio = GetActorLocation();

    ruta.Add(inicio);
    ruta.Add(inicio + FVector(300, 0, 0));
    ruta.Add(inicio + FVector(300, 300, 0));
    ruta.Add(inicio + FVector(600, 300, 0));
    ruta.Add(inicio + FVector(600, 0, 0));
    ruta.Add(inicio + FVector(900, 0, 0));
}