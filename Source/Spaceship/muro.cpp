#include "muro.h"
#include "Components/StaticMeshComponent.h"

AMuro::AMuro()
{
    PrimaryActorTick.bCanEverTick = true;

    meshMuro = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshMuro"));
    RootComponent = meshMuro;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
        TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    if (MeshAsset.Succeeded())
    {
        meshMuro->SetStaticMesh(MeshAsset.Object);
    }

    meshMuro->SetWorldScale3D(FVector(5.0f, 0.5f, 2.0f));
    vida = 100.0f;
}

void AMuro::BeginPlay()
{
    Super::BeginPlay();
}

void AMuro::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    EfectoEspecial(DeltaTime);
}

void AMuro::EfectoEspecial(float DeltaTime)
{
    // Base no hace nada, las hijas lo sobreescriben
}