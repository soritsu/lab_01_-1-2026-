
#include "enemigo.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
Aenemigo::Aenemigo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	meshEnemy = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshEnemy"));
	meshEnemy->SetSimulatePhysics(false);
	meshEnemy->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
}

// ---------------------------------------------------------------------------
// Pool
// ===========================================================================
void Aenemigo::ActivateFromPool(USplineComponent* InSpline, int32 InIndex, float InSpawnDelay)
{
	if (!InSpline) return;

	EntrySplineRef = InSpline;
	EntrySplineLength = InSpline->GetSplineLength();
	DistanceAlongSpline = 0.f;
	EnemyIndex = InIndex;

	// Cada enemigo arranca con una fase senoidal distinta → no se mueven igual
	LibrePhase = EnemyIndex * 0.45f;
	LibreTimer = 0.f;
	EsperaTimer = 0.f;
	bIsActive = true;
	State = EEnemigoState::EnPool;

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	if (InSpawnDelay > 0.f)
	{
		GetWorldTimerManager().SetTimer(
			StartDelayHandle, this,
			&Aenemigo::StartMoving,
			InSpawnDelay, false
		);
	}
	else
	{
		StartMoving();
	}
}

void Aenemigo::StartMoving()
{
	State = EEnemigoState::Entrando;
	DistanceAlongSpline = 0.f;

	FVector  Pos = EntrySplineRef->GetLocationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World);
	FRotator Rot = EntrySplineRef->GetRotationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World);
	SetActorLocationAndRotation(Pos, Rot);
}

void Aenemigo::ReturnToPool()
{
	bIsActive = false;
	State = EEnemigoState::EnPool;

	GetWorldTimerManager().ClearTimer(StartDelayHandle);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

// ===========================================================================
// Tick — dispatcher
// ===========================================================================
void Aenemigo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
	case EEnemigoState::Entrando:  TickEntrando(DeltaTime); break;
	case EEnemigoState::Libre:     TickLibre(DeltaTime); break;
	case EEnemigoState::Formacion: TickFormacion(DeltaTime); break;
	case EEnemigoState::Espera:    TickEspera(DeltaTime); break;
	default: break;
	}
}

// ===========================================================================
// Fase 1 — ENTRANDO
// ===========================================================================
void Aenemigo::TickEntrando(float DeltaTime)
{
	if (!EntrySplineRef) return;

	DistanceAlongSpline += MoveSpeed * DeltaTime;

	if (DistanceAlongSpline >= EntrySplineLength)
	{
		// Llegó al final de la spline → pasa a movimiento libre
		FVector EndPos = EntrySplineRef->GetLocationAtDistanceAlongSpline(
			EntrySplineLength, ESplineCoordinateSpace::World);
		SetActorLocation(EndPos);
		GoToLibre();
		return;
	}

	FVector  Pos = EntrySplineRef->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	FRotator Rot = EntrySplineRef->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	SetActorLocationAndRotation(Pos, Rot);
}

// ===========================================================================
// Fase 2 — LIBRE
// ===========================================================================
void Aenemigo::GoToLibre()
{
	State = EEnemigoState::Libre;
	LibreTimer = 0.f;
	LibreOrigin = GetActorLocation();

	// Dirección base: deriva suavemente hacia el jugador (Y negativo en top-down)
	// Pequeña variación lateral por índice para que no se apilen
	const float LateralBias = FMath::Sin(EnemyIndex * 1.3f) * 0.4f;
	LibreDir = FVector(-1.f, LateralBias, 0.f).GetSafeNormal();
}

void Aenemigo::TickLibre(float DeltaTime)
{
	LibreTimer += DeltaTime;
	LibrePhase += DeltaTime * ZigZagFrequency * 2.f * PI;

	// Movimiento base: deriva en LibreDir
	FVector BaseDelta = LibreDir * FreeSpeed * DeltaTime;

	// Zig-zag perpendicular: seno con fase personal → cada nave oscila distinto
	FVector Perp = FVector(1.f, 0.f, 0.f);  // perpendicular en plano XY top-down
	float   ZigOffset = FMath::Sin(LibrePhase) * ZigZagAmplitude * DeltaTime;
	FVector ZigDelta = Perp * ZigOffset;

	FVector NewPos = GetActorLocation() + BaseDelta + ZigDelta;

	// Rotar para mirar hacia donde va
	FVector MoveDir = (BaseDelta + ZigDelta).GetSafeNormal();
	if (!MoveDir.IsNearlyZero())
	{
		FRotator NewRot = MoveDir.ToOrientationRotator();
		SetActorLocationAndRotation(NewPos, NewRot);
	}
	else
	{
		SetActorLocation(NewPos);
	}
}

// ===========================================================================
// Fase 3 — FORMACION
// ===========================================================================
void Aenemigo::GoToFormation(const FVector& InSlotWorld)
{
	TargetSlot = InSlotWorld;
	State = EEnemigoState::Formacion;
}

void Aenemigo::TickFormacion(float DeltaTime)
{
	FVector Current = GetActorLocation();
	FVector ToSlot = TargetSlot - Current;
	float   Dist = ToSlot.Size();

	if (Dist < 8.f)
	{
		// Llegó al slot con precisión suficiente
		SetActorLocation(TargetSlot);
		SetActorRotation(FRotator::ZeroRotator);

		State = EEnemigoState::Espera;
		EsperaTimer = 0.f;
		return;
	}

	// Movimiento a velocidad constante hacia el slot
	FVector Dir = ToSlot / Dist;
	float   Step = FMath::Min(FormationSpeed * DeltaTime, Dist);
	FVector NewPos = Current + Dir * Step;

	FRotator NewRot = Dir.ToOrientationRotator();
	SetActorLocationAndRotation(NewPos, NewRot);
}

// ===========================================================================
// Fase 4 — ESPERA
// ===========================================================================
void Aenemigo::TickEspera(float DeltaTime)
{
	EsperaTimer += DeltaTime;

	if (EsperaTimer >= EsperaTime)
	{
		GoToLibre();
	}
}