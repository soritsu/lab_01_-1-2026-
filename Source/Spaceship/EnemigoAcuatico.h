
#pragma once

#include "CoreMinimal.h"
#include "enemigo.h"
#include "EnemigoAcuatico.generated.h"

/**
 * Enemigo acuático — comportamiento polimórfico en formación
 */
	UCLASS()
	class SPACESHIP_API AEnemigoAcuatico : public Aenemigo
{
	GENERATED_BODY()

public:
	AEnemigoAcuatico();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Override de formación: añade patrón de "nado" mientras se aproxima al slot
	virtual void GoToFormation(const FVector& InSlotWorld) override;
	virtual void TickFormacion(float DeltaTime) override;
	virtual void TickEspera(float DeltaTime) override;

	// Parámetros específicos
	UPROPERTY(EditAnywhere, Category = "Acuatico")
	float SwimAmplitude = 40.f;

	UPROPERTY(EditAnywhere, Category = "Acuatico")
	float SwimFrequency = 2.0f;

private:
	// fase para el balanceo de nado
	float SwimPhase = 0.f;
};