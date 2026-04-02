
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "enemigo.generated.h"


UENUM(BlueprintType)
enum class EEnemigoState : uint8
{
	EnPool,      // Inactivo, sin tick
	Entrando,    // Sigue spline de entrada curva
	Libre,       // Zig-zag senoidal independiente
	Formacion,   // Lerp suave hacia slot frente al Pawn
	Espera,      // Quieto en slot, cuenta regresiva
};
UCLASS()
class SPACESHIP_API Aenemigo : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	Aenemigo();
	virtual void Tick(float DeltaTime) override;


	// Activa el enemigo con su spline de entrada y su índice único
	void ActivateFromPool(USplineComponent* InSpline, int32 InIndex, float InSpawnDelay);

	// Ordena ir a formación — el Spawner calcula el slot y lo pasa aquí
	virtual void GoToFormation(const FVector& InSlotWorld);

	// Ordena volver al movimiento libre después de la espera
	virtual void GoToLibre();

	// Devuelve al pool sin destruir el actor
	virtual void ReturnToPool();

	// -----------------------------------------------------------------------
	// Parámetros — todos configurables desde ASpawner
	// -----------------------------------------------------------------------
	float MoveSpeed = 400.f;  // velocidad sobre spline de entrada
	float FreeSpeed = 180.f;  // velocidad de drift en fase libre
	float FormationSpeed = 300.f;  // velocidad de lerp hacia el slot
	float LibreTime = 5.f;    // segundos en libre antes de ir a formación
	float EsperaTime = 5.f;    // segundos quieto en formación

	float ZigZagAmplitude = 120.f;  // ancho del zig-zag en unidades UE
	float ZigZagFrequency = 0.8f;   // oscilaciones por segundo

	bool bIsActive = false;
	EEnemigoState  State = EEnemigoState::EnPool;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* meshEnemy;

	// Un método Tick por estado — protegido para permitir override en subclases
	virtual void TickEntrando(float DeltaTime);
	virtual void TickLibre(float DeltaTime);
	virtual void TickFormacion(float DeltaTime);
	virtual void TickEspera(float DeltaTime);


// Spline de entrada
	UPROPERTY()
	USplineComponent* EntrySplineRef = nullptr;
	float DistanceAlongSpline = 0.f;
	float EntrySplineLength = 0.f;

	// Fase Libre
	int32   EnemyIndex = 0;        // da variación única (fase, dirección) a cada nave
	float   LibreTimer = 0.f;
	float   LibrePhase = 0.f;      // fase senoidal personal, arranca distinta por índice
	FVector LibreOrigin = FVector::ZeroVector;
	FVector LibreDir = FVector(0.f, -1.f, 0.f); // dirección base del drift

	// Formación / Espera
	FVector TargetSlot = FVector::ZeroVector;
	float   EsperaTimer = 0.f;

	// Delay de entrada escalonado
	FTimerHandle StartDelayHandle;
	void StartMoving();


private:


	
};