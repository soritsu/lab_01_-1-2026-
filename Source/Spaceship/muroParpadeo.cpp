#include "muroParpadeo.h"
#include "Components/StaticMeshComponent.h"

AMuroParpadeo::AMuroParpadeo()
{
    tiempoAcumulado = 0.0f;
    intervalo = 2.0f;
    visible = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
        TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
    if (MeshAsset.Succeeded())
    {
        meshMuro->SetStaticMesh(MeshAsset.Object);
    }

    meshMuro->SetWorldScale3D(FVector(4.0f, 0.5f, 2.0f));
    

}

void AMuroParpadeo::EfectoEspecial(float DeltaTime)
{
    tiempoAcumulado += DeltaTime;

    if (tiempoAcumulado >= intervalo)
    {
        tiempoAcumulado = 0.0f;
        visible = !visible;
        meshMuro->SetVisibility(visible);
        meshMuro->SetCollisionEnabled(visible
            ? ECollisionEnabled::QueryAndPhysics
            : ECollisionEnabled::NoCollision);
    }
}