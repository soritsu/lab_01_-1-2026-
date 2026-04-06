#include "SpaceshipGameMode.h"
#include "SpaceshipPawn.h"
#include "enemigo.h"
#include "alien.h"
#include "meteorito.h"
#include "muro.h"
#include "muroMovil.h"
#include "muroParpadeo.h"

ASpaceshipGameMode::ASpaceshipGameMode()
{
    DefaultPawnClass = ASpaceshipPawn::StaticClass();
}

void ASpaceshipGameMode::BeginPlay()
{
    Super::BeginPlay();

    // ---- NAVES ----
    AActor* enemigo = GetWorld()->SpawnActor<Aenemigo>(
        Aenemigo::StaticClass(),
        FVector(0.0f, -500.0f, 160.0f),
        FRotator::ZeroRotator);
    naves.Add(enemigo);

    AActor* alien = GetWorld()->SpawnActor<AAlien>(
        AAlien::StaticClass(),
        FVector(0.0f, -500.0f, 160.0f),
        FRotator::ZeroRotator);
    naves.Add(alien);

    AActor* meteorito = GetWorld()->SpawnActor<AMeteorito>(
        AMeteorito::StaticClass(),
        FVector(500.0f, -500.0f, 800.0f),
        FRotator::ZeroRotator);
    naves.Add(meteorito);

    // ---- MUROS ----
    for (int i = 0; i <= 3; i++) {
        AActor* muro = GetWorld()->SpawnActor<AMuro>(
            AMuro::StaticClass(),
            FVector(-550.0f, 173.0f-500*i, 160.0f),
            FRotator(0.0f,90.0f,0.0f));
        if (muro)
        {
            muros.Add(muro);
        }
    }
    for (int i = 0; i <= 3; i++) {
        AActor* muro = GetWorld()->SpawnActor<AMuro>(
            AMuro::StaticClass(),
            FVector(-300 + 500 * i, -1623.0f, 160.0f),
            FRotator::ZeroRotator);
        if (muro)
        {
            muros.Add(muro);
        }
    }
    for (int i = 0; i <= 4; i++) {
        AActor* muro = GetWorld()->SpawnActor<AMuro>(
            AMuro::StaticClass(),
		    FVector(1450.0f, -1373.0f+500*i, 160.0f),
		    FRotator(0.0f, 90.0f, 0.0f));
        if (muro)
        {
            muros.Add(muro);
        }
    };
    for (int i = 0; i <= 2; i++) {
        AActor* muro = GetWorld()->SpawnActor<AMuro>(
            AMuro::StaticClass(),
            FVector(1200.0f-500*i, 877.0f, 160.0f),
            FRotator::ZeroRotator);
        if (muro)
        {
            muros.Add(muro);
        }
    }
    for (int i = 0; i <= 3; i++) {
        AActor* muro = GetWorld()->SpawnActor<AMuro>(
            AMuro::StaticClass(),
            FVector(-50, 627.0f-500*i, 160.0f),
            FRotator(0.0f, 90.0f, 0.0f));
        if (muro)
        {
            muros.Add(muro);
        }
    }
    for (int i = 0; i < 2; i++) {
        AActor* muro = GetWorld()->SpawnActor<AMuro>(
            AMuro::StaticClass(),
            FVector(200 + 500 * i, -1123.0f, 160.0f),
            FRotator::ZeroRotator);
        if (muro)
        {
            muros.Add(muro);
        }
    }
    for (int i = 0; i <= 2; i++) {
        AActor* muro = GetWorld()->SpawnActor<AMuro>(
            AMuro::StaticClass(),
            FVector(950.0f, -877.0f + 500 * i, 160.0f),
            FRotator(0.0f, 90.0f, 0.0f));
        if (muro)
        {
            muros.Add(muro);
        }
    }
    for (int i = 0; i < 1; i++) {
        AActor* muro = GetWorld()->SpawnActor<AMuro>(
            AMuro::StaticClass(),
            FVector(700 - 500 * i, 373.0f, 160.0f),
            FRotator::ZeroRotator);
        if (muro)
        {
            muros.Add(muro);
        }
    }


	// ---- MURO MOVIL ----
    AActor* muroMovil = GetWorld()->SpawnActor<AMuroMovil>(
        AMuroMovil::StaticClass(),
        FVector(-300.0f, 373.0f, 160.0f),
        FRotator::ZeroRotator);
    if (muroMovil)
    {
        muros.Add(muroMovil);
	}
	// ---- MURO PARPADEO ----
    AActor* muroParpadeo = GetWorld()->SpawnActor<AMuroParpadeo>(
        AMuroParpadeo::StaticClass(),
        FVector(1200.0f, -1123.0f, 160.0f),
        FRotator::ZeroRotator);
    if (muroParpadeo)
    {
        muros.Add(muroParpadeo);
	}
}