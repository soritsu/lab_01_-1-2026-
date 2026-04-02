
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Spawner.h"
#include "EnemigoAcuatico.h" // para asignar valor por defecto de AquaticEnemyClass
#include "Math/UnrealMathUtility.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// RootComponent neutro
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Clases por defecto (puedes cambiarlas en el editor)
	NormalEnemyClass = Aenemigo::StaticClass();
	AquaticEnemyClass = AEnemigoAcuatico::StaticClass();
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	// Buscamos el pawn del jugador (twin-stick starter content usa el pawn por defecto)
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASpawner: No se encontró PlayerPawn. Formación usará origen (0,0,0)."));
	}

	BuildEntrySplines();
	InitPool();
}

void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bWaveActive) return;

	LibreTimer += DeltaTime;

	// Después de LibreTime segundos todos van a formación (una sola vez por oleada)
	if (!bFormationSent && LibreTimer >= EnemyLibreTime)
	{
		bFormationSent = true;
		SendAllToFormation();
	}
}

// ===========================================================================
// Splines de entrada
// Generamos dos conjuntos: top (original) y bottom (espejo para acuáticos).
// ===========================================================================
void ASpawner::BuildEntrySplines()
{
	EntrySplinesTop.Empty();
	EntrySplinesBottom.Empty();

	// Puntos base de cada spline — col 0 (izquierda) a col 3 (derecha)
	TArray<FVector> S0 = {
		FVector(1900.f, -600.f, 100.f),
		FVector(1400.f, -500.f, 100.f),
		FVector(1100.f, -300.f, 100.f),
		FVector(800.f,  -50.f, 100.f),
		FVector(660.f,  -225.f, 100.f),   // termina cerca del slot col 0
	};
	TArray<FVector> S1 = {
		FVector(1900.f, -200.f, 100.f),
		FVector(1500.f, -100.f, 100.f),
		FVector(1200.f, 100.f, 100.f),
		FVector(900.f,  50.f, 100.f),
		FVector(660.f,  -75.f, 100.f),   // slot col 1
	};
	TArray<FVector> S2 = {
		FVector(1900.f, 200.f, 100.f),
		FVector(1500.f, 100.f, 100.f),
		FVector(1200.f, -100.f, 100.f),
		FVector(900.f,  -50.f, 100.f),
		FVector(660.f,  75.f, 100.f),   // slot col 2
	};
	TArray<FVector> S3 = {
		FVector(1900.f, 600.f, 100.f),
		FVector(1400.f, 500.f, 100.f),
		FVector(1100.f, 300.f, 100.f),
		FVector(800.f,  50.f, 100.f),
		FVector(660.f,  225.f, 100.f),   // slot col 3
	};

	TArray<TArray<FVector>> AllPoints = { S0, S1, S2, S3 };

	// Construir top
	for (int32 c = 0; c < 4; c++)
	{
		FString Name = FString::Printf(TEXT("EntrySplineTop_%d"), c);
		USplineComponent* Spline = NewObject<USplineComponent>(this, *Name);
		Spline->SetupAttachment(RootComponent);
		Spline->RegisterComponent();
		Spline->ClearSplinePoints(false);

		const TArray<FVector>& Pts = AllPoints[c];
		for (int32 i = 0; i < Pts.Num(); i++)
		{
			FSplinePoint SP;
			SP.Position = Pts[i];
			SP.Type = ESplinePointType::Curve;
			SP.InputKey = static_cast<float>(i);
			Spline->AddPoint(SP, false);
		}
		Spline->UpdateSpline();
		EntrySplinesTop.Add(Spline);
	}

	// Construir bottom como espejo en X (vienen desde el otro lado / "abajo")
	for (int32 c = 0; c < 4; c++)
	{
		FString Name = FString::Printf(TEXT("EntrySplineBottom_%d"), c);
		USplineComponent* Spline = NewObject<USplineComponent>(this, *Name);
		Spline->SetupAttachment(RootComponent);
		Spline->RegisterComponent();
		Spline->ClearSplinePoints(false);

		const TArray<FVector>& Pts = AllPoints[c];
		for (int32 i = 0; i < Pts.Num(); i++)
		{
			FSplinePoint SP;
			// Espejar X para que entren desde el lado opuesto (ajusta si prefieres otra dirección)
			SP.Position = FVector(-Pts[i].X, Pts[i].Y, Pts[i].Z);
			SP.Type = ESplinePointType::Curve;
			SP.InputKey = static_cast<float>(i);
			Spline->AddPoint(SP, false);
		}
		Spline->UpdateSpline();
		EntrySplinesBottom.Add(Spline);
	}

	UE_LOG(LogTemp, Log, TEXT("ASpawner: %d top splines y %d bottom splines construidas."), EntrySplinesTop.Num(), EntrySplinesBottom.Num());
}

// ===========================================================================
// Pool — ahora inicializamos dos pools distintos
// ===========================================================================
void ASpawner::InitPool()
{
	EnemyPoolNormal.Reserve(PoolSizeNormal);
	EnemyPoolAcuatico.Reserve(PoolSizeAcuatico);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Normal
	TSubclassOf<Aenemigo> ToSpawnNormal = NormalEnemyClass ? NormalEnemyClass : Aenemigo::StaticClass();
	for (int32 i = 0; i < PoolSizeNormal; i++)
	{
		Aenemigo* Enemy = GetWorld()->SpawnActor<Aenemigo>(
			ToSpawnNormal,
			FVector::ZeroVector, FRotator::ZeroRotator, Params
		);

		if (Enemy)
		{
			Enemy->MoveSpeed = EnemyMoveSpeed;
			Enemy->FreeSpeed = EnemyFreeSpeed;
			Enemy->FormationSpeed = EnemyFormationSpeed;
			Enemy->LibreTime = EnemyLibreTime;
			Enemy->EsperaTime = EnemyEsperaTime;
			Enemy->ReturnToPool();
			EnemyPoolNormal.Add(Enemy);
		}
	}

	// Acuático
	TSubclassOf<Aenemigo> ToSpawnAcu = AquaticEnemyClass ? AquaticEnemyClass : AEnemigoAcuatico::StaticClass();
	for (int32 i = 0; i < PoolSizeAcuatico; i++)
	{
		Aenemigo* Enemy = GetWorld()->SpawnActor<Aenemigo>(
			ToSpawnAcu,
			FVector::ZeroVector, FRotator::ZeroRotator, Params
		);

		if (Enemy)
		{
			Enemy->MoveSpeed = EnemyMoveSpeed;
			Enemy->FreeSpeed = EnemyFreeSpeed;
			Enemy->FormationSpeed = EnemyFormationSpeed;
			Enemy->LibreTime = EnemyLibreTime;
			Enemy->EsperaTime = EnemyEsperaTime;
			Enemy->ReturnToPool();
			EnemyPoolAcuatico.Add(Enemy);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("ASpawner: Pool normal %d, acuático %d."), EnemyPoolNormal.Num(), EnemyPoolAcuatico.Num());
}

// ===========================================================================
// LaunchWave — lanzamos ambas colecciones; acuáticos usan splines bottom
// ===========================================================================
void ASpawner::LaunchWave()
{
	// Validaciones de splines
	if (EntrySplinesTop.Num() < 4 || EntrySplinesBottom.Num() < 4)
	{
		UE_LOG(LogTemp, Error, TEXT("ASpawner: Splines no inicializadas (top/bottom)."));
		return;
	}

	int32 Count = 0;
	// Lanzar normales
	for (Aenemigo* Enemy : EnemyPoolNormal)
	{
		if (!Enemy || Enemy->bIsActive) continue;
		if (Count >= PoolSizeNormal) break;

		const int32  Col = Count % 4;
		const float  Delay = Count * DelayBetweenEnemies;
		USplineComponent* Spline = EntrySplinesTop[Col];

		Enemy->ActivateFromPool(Spline, Count, Delay);
		Count++;
	}

	// Reiniciar contador para acuáticos (para dar columnas separadas)
	Count = 0;
	for (Aenemigo* Enemy : EnemyPoolAcuatico)
	{
		if (!Enemy || Enemy->bIsActive) continue;
		if (Count >= PoolSizeAcuatico) break;

		const int32 Col = Count % 4;
		// Opcional: separo delays para que no coincidan exactamente con los normales
		const float Delay = (Count * DelayBetweenEnemies) + 0.5f; // ligero offset
		USplineComponent* Spline = EntrySplinesBottom[Col];

		Enemy->ActivateFromPool(Spline, Count + PoolSizeNormal, Delay);
		Count++;
	}

	bWaveActive = true;
	bFormationSent = false;
	LibreTimer = 0.f;
	SetActorTickEnabled(true);

	UE_LOG(LogTemp, Log, TEXT("ASpawner: Oleada lanzada — normales %d, acuáticos %d."), EnemyPoolNormal.Num(), EnemyPoolAcuatico.Num());
}

// ===========================================================================
// Formación — cuadrícula para normales y semicircunferencia para acuáticos
// ===========================================================================
FVector ASpawner::GetSlotPosition(int32 Col, int32 Row) const
{
	// Centro de la formación: posición actual del Pawn + offset hacia arriba
	FVector Origin = PlayerPawn
		? PlayerPawn->GetActorLocation() + FVector(FormationYOffset, 0.f, 100.f)
		: FVector(FormationYOffset, 0.f, 100.f);

	const float TotalWidth = 3.f * ColSpacing;
	const float StartX = Origin.X - TotalWidth * 0.5f;
	const float StartY = Origin.Y + (4.f * RowSpacing * 0.5f);

	return FVector(
		StartX + Col * ColSpacing,
		StartY - Row * RowSpacing,
		100.f
	);
}

FVector ASpawner::GetAquaticFormationPosition(int32 Index, int32 Total) const
{
	// Centro de la formación acuática: por debajo del jugador (ajusta offset si lo deseas)
	FVector Center = PlayerPawn
		? PlayerPawn->GetActorLocation() + FVector(FormationYOffset, -400.f, 100.f)
		: FVector(FormationYOffset, -400.f, 100.f);

	// Radio y ángulos para semicircunferencia
	const float Radius = 350.f;
	if (Total <= 1)
	{
		return Center;
	}

	// Distribuimos en semicircunferencia de -PI/2 a PI/2
	const float StartAngle = -PI / 2.0f;
	const float EndAngle = PI / 2.0f;
	const float Step = (EndAngle - StartAngle) / (Total + 1);

	float Angle = StartAngle + (Index + 1) * Step;

	float X = Center.X + Radius * FMath::Cos(Angle);
	float Y = Center.Y + Radius * FMath::Sin(Angle);

	return FVector(X, Y, 100.f);
}

void ASpawner::SendAllToFormation()
{
	// Normales
	for (Aenemigo* Enemy : EnemyPoolNormal)
	{
		if (!Enemy || !Enemy->bIsActive) continue;

		const int32 Idx = EnemyPoolNormal.IndexOfByKey(Enemy);
		if (Idx == INDEX_NONE) continue;
		const int32 Col = Idx % 4;
		const int32 Row = Idx / 4;

		Enemy->GoToFormation(GetSlotPosition(Col, Row));
	}

	// Acuáticos — usan formación semicircular "abajo"
	for (Aenemigo* Enemy : EnemyPoolAcuatico)
	{
		if (!Enemy || !Enemy->bIsActive) continue;

		const int32 Idx = EnemyPoolAcuatico.IndexOfByKey(Enemy);
		if (Idx == INDEX_NONE) continue;

		FVector AquaticTarget = GetAquaticFormationPosition(Idx, EnemyPoolAcuatico.Num());
		Enemy->GoToFormation(AquaticTarget);
	}

	UE_LOG(LogTemp, Log, TEXT("ASpawner: Formación enviada (normales + acuáticos)."));
}