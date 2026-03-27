// Fill out your copyright notice in the Description page of Project Settings.


#include "enemigo.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
Aenemigo::Aenemigo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshEnemy = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshEnemy"));
	RootComponent = meshEnemy;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshEnemyAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (MeshEnemyAsset.Succeeded())
	{
		meshEnemy->SetStaticMesh(MeshEnemyAsset.Object);
	}
}

// Called when the game starts or when spawned
void Aenemigo::BeginPlay()
{
	Super::BeginPlay();
	CargarRuta();
	
}

// Called every frame
void Aenemigo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Mover(DeltaTime);
}
void Aenemigo::CargarRuta()
{
    PosicionInicial = GetActorLocation();
    PuntosRuta.Empty();

    // Agregar punto inicial
    PuntosRuta.Add(PosicionInicial);

    // Generar 10 waypoints aleatorios (líneas rectas H/V/D)
    for (int32 i = 0; i < 10; i++)
    {
        float X = FMath::RandRange(WorldLimitesMin.X, WorldLimitesMax.X);
        float Y = FMath::RandRange(WorldLimitesMin.Y, WorldLimitesMax.Y);
        FVector NuevoPuntoRuta(X, Y, PosicionInicial.Z);  // Mantiene altura
        PuntosRuta.Add(NuevoPuntoRuta);
    }

    IndicePuntoRutaActual = 1;
}

void Aenemigo::Mover(float DeltaTime)
{

    if (!bMovimientoAutonomo)
    {
        FVector PosicionActual = GetActorLocation();

        float Distancia = FVector::Dist(PosicionActual, PosicionDestinoGameMode);

        if (Distancia <= Tolerancia)
        {
            // Waypoint alcanzado, siguiente
            return;
        }
        else
        {
            // Mover en línea recta
            FVector Direccion = (PosicionDestinoGameMode - PosicionActual).GetSafeNormal();
            FVector NuevaUbicacion = PosicionActual + (Direccion * VelocidadMovimiento * DeltaTime);
            SetActorLocation(NuevaUbicacion);
        }
    }
    else if (PuntosRuta.Num() > 1)
    {
        FVector UbicacionActual = GetActorLocation();
        FVector UbicacionDestino = PuntosRuta[IndicePuntoRutaActual];

        float Distancia = FVector::Dist(UbicacionActual, UbicacionDestino);

        if (Distancia <= Tolerancia)
        {
            // Waypoint alcanzado, siguiente
            IndicePuntoRutaActual++;
            if (IndicePuntoRutaActual >= PuntosRuta.Num())
            {
                // Volver al inicio
                IndicePuntoRutaActual = 0;
            }
        }
        else
        {
            // Mover en línea recta
            FVector Direccion = (UbicacionDestino - UbicacionActual).GetSafeNormal();
            FVector NuevaUbicacion = UbicacionActual + (Direccion * VelocidadMovimiento * DeltaTime);
            SetActorLocation(NuevaUbicacion);
        }
    }

}

