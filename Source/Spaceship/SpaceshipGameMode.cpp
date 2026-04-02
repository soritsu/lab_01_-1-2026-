#include "SpaceshipGameMode.h"
#include "SpaceshipPawn.h"
#include "enemigo.h"

ASpaceshipGameMode::ASpaceshipGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ASpaceshipPawn::StaticClass();
}
void ASpaceshipGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Delay de 0.3s: da tiempo al Pawn del twin-stick de spawnearse
	// antes de que el Spawner intente buscarlo con GetPlayerPawn()
	FTimerHandle InitHandle;
	GetWorldTimerManager().SetTimer(
		InitHandle, this,
		&ASpaceshipGameMode::InitSpawner,
		0.3f, false
	);
}

void ASpaceshipGameMode::InitSpawner()
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	EnemySpawner = GetWorld()->SpawnActor<ASpawner>(
		ASpawner::StaticClass(),
		FVector::ZeroVector, FRotator::ZeroRotator, Params
	);

	if (!EnemySpawner)
	{
		UE_LOG(LogTemp, Error, TEXT("SpaceshipGameMode: No se pudo crear ASpawner."));
		return;
	}

	// -----------------------------------------------------------------------
	// Aquí están TODOS los parámetros del juego en un solo lugar.
	// Ajusta estos números sin tocar enemigo.cpp ni Spawner.cpp.
	// -----------------------------------------------------------------------

	// Pools: 10 normales + 10 acuáticos
	EnemySpawner->PoolSizeNormal = 10;
	EnemySpawner->PoolSizeAcuatico = 10;

	EnemySpawner->DelayBetweenEnemies = 0.18f;   // entrada escalonada rápida
	EnemySpawner->EnemyMoveSpeed = 420.f;   // velocidad en la spline de entrada
	EnemySpawner->EnemyFreeSpeed = 180.f;   // velocidad de drift libre
	EnemySpawner->EnemyFormationSpeed = 320.f;   // velocidad al ir al slot
	EnemySpawner->EnemyLibreTime = 5.f;     // segundos libres antes de formar
	EnemySpawner->EnemyEsperaTime = 5.f;     // segundos quietos en formación

	EnemySpawner->FormationYOffset = 600.f;   // distancia frente al pawn
	EnemySpawner->ColSpacing = 150.f;
	EnemySpawner->RowSpacing = 120.f;

	// BeginPlay del Spawner ya corrió durante SpawnActor:
	// pool y splines ya están listos → podemos lanzar directo
	EnemySpawner->LaunchWave();
}