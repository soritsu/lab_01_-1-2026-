
#include "EnemigoAcuatico.h"
#include "Math/UnrealMathUtility.h"

AEnemigoAcuatico::AEnemigoAcuatico()
{
	// valores por defecto distintos para que sea más "acuático"
	FormationSpeed = 220.f;
	FreeSpeed = 140.f;
	SwimAmplitude = 40.f;
	SwimFrequency = 2.0f;
}

void AEnemigoAcuatico::BeginPlay()
{
	Super::BeginPlay();
	// fase inicial distinta según índice para no moverse sincronizados
	SwimPhase = FMath::FRandRange(0.f, 2.f * PI);
}

void AEnemigoAcuatico::GoToFormation(const FVector& InSlotWorld)
{
	// Podemos ajustar target y estado igual que base, pero iniciar fase de nado
	TargetSlot = InSlotWorld;
	State = EEnemigoState::Formacion;
	SwimPhase = FMath::FRandRange(0.f, 2.f * PI);
}

void AEnemigoAcuatico::TickFormacion(float DeltaTime)
{
	// Mueve hacia el slot pero con oscilación lateral tipo nadar
	FVector Current = GetActorLocation();
	FVector ToSlot = TargetSlot - Current;
	float Dist = ToSlot.Size();

	if (Dist < 8.f)
	{
		SetActorLocation(TargetSlot);
		SetActorRotation(FRotator::ZeroRotator);

		State = EEnemigoState::Espera;
		EsperaTimer = 0.f;
		return;
	}

	// Movimiento base hacia slot
	FVector Dir = ToSlot / Dist;
	float Step = FMath::Min(FormationSpeed * DeltaTime, Dist);
	FVector BaseNewPos = Current + Dir * Step;

	// Oscilación perpendicular (usar Y como lateral en top-down)
	SwimPhase += DeltaTime * SwimFrequency * 2.f * PI;
	FVector Perp = FVector(-Dir.Y, Dir.X, 0.f).GetSafeNormal();
	FVector SwimOffset = Perp * FMath::Sin(SwimPhase) * SwimAmplitude * DeltaTime;

	FVector NewPos = BaseNewPos + SwimOffset;

	FRotator NewRot = (Dir).ToOrientationRotator();
	SetActorLocationAndRotation(NewPos, NewRot);
}

void AEnemigoAcuatico::TickEspera(float DeltaTime)
{
	// Mientras espera puede "flotar" ligeramente (pequeña oscilación Z)
	EsperaTimer += DeltaTime;

	FVector Pos = GetActorLocation();
	float FloatOffset = FMath::Sin(EsperaTimer * SwimFrequency * 0.5f) * (SwimAmplitude * 0.03f);
	Pos.Z += FloatOffset;
	SetActorLocation(Pos);

	if (EsperaTimer >= EsperaTime)
	{
		GoToLibre();
	}
}