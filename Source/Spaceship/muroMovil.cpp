#include "muroMovil.h"
#include "Components/StaticMeshComponent.h"

AMuroMovil::AMuroMovil()
{
    velocidad = 300.0f;
    yendo = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
        TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
    if (MeshAsset.Succeeded())
    {
        meshMuro->SetStaticMesh(MeshAsset.Object);
    }

    meshMuro->SetWorldScale3D(FVector(3.0f, 3.0f, 2.0f));
}
void AMuroMovil::BeginPlay()
{
    Super::BeginPlay();
    puntoA = GetActorLocation();
    puntoB = puntoA + FVector(800.0f, 0.0f, 0.0f);
}
void AMuroMovil::EfectoEspecial(float DeltaTime)
{
	FVector destino = yendo ? puntoB : puntoA;// Si yendo es true, el destino es puntoB, si es false, el destino es puntoA
    FVector nuevaPos = FMath::VInterpConstantTo(
        GetActorLocation(), destino, DeltaTime, velocidad);
    SetActorLocation(nuevaPos);

    if (GetActorLocation().Equals(destino, 5.0f))// Si el muro ha llegado al destino (con una tolerancia de 5 unidades), cambia la dirección
    {
        yendo = !yendo;
    }
}

