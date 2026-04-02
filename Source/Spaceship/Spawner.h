
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "enemigo.h"
#include "Spawner.generated.h"

UCLASS()
class SPACESHIP_API ASpawner : public AActor
{
	GENERATED_BODY()

public:
	ASpawner();

	// Lanza la oleada de entrada (llama una sola vez al inicio)
	void LaunchWave();

	
	int32   PoolSizeNormal = 10;
	int32   PoolSizeAcuatico = 10;

	float   DelayBetweenEnemies = 0.18f;  // segundos entre cada entrada
	float   EnemyMoveSpeed = 420.f;
	float   EnemyFreeSpeed = 180.f;
	float   EnemyFormationSpeed = 320.f;
	float   EnemyLibreTime = 5.f;    // segundos en libre antes de formación
	float   EnemyEsperaTime = 10.f;    // segundos quietos en formación

	// Formación 4×5 centrada en el Pawn
	// Offset desde el Pawn: cuánto "arriba" (Y+) aparece la cuadrícula
	float   FormationYOffset = -100.f;
	float   ColSpacing = 150.f;
	float   RowSpacing = 120.f;

	// Clases editables para spawnear
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<Aenemigo> NormalEnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<Aenemigo> AquaticEnemyClass;

	// Referencia al Pawn — se busca automáticamente en BeginPlay
	UPROPERTY()
	APawn* PlayerPawn = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// Splines de entrada superiores (para enemigos normales)
	UPROPERTY()
	TArray<USplineComponent*> EntrySplinesTop;  // una spline por columna (4 splines)

	// Splines de entrada inferiores (para enemigos acuáticos)
	UPROPERTY()
	TArray<USplineComponent*> EntrySplinesBottom;  // espejo / "desde abajo"

	// Pools separados
	UPROPERTY()
	TArray<Aenemigo*> EnemyPoolNormal;

	UPROPERTY()
	TArray<Aenemigo*> EnemyPoolAcuatico;

	// Timer que gestiona cuándo ordenar formación
	float LibreTimer = 0.f;
	bool  bWaveActive = false;
	bool  bFormationSent = false;

	void BuildEntrySplines();
	void InitPool();

	// Calcula el slot world de (col, fila) relativo al Pawn actual (cuadrícula)
	FVector GetSlotPosition(int32 Col, int32 Row) const;

	// Calcula posición de formación específica para acuáticos (semicircunferencia "abajo")
	FVector GetAquaticFormationPosition(int32 Index, int32 Total) const;

	// Manda a todos los enemigos activos a formación
	void SendAllToFormation();
};