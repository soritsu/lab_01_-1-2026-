// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "enemigo.generated.h"

UCLASS()

class SPACESHIP_API Aenemigo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aenemigo();
	float VelocidadMovimiento = 100.0f;

	FVector WorldLimitesMin = FVector(-1000, -1000, 0);
	FVector WorldLimitesMax = FVector(1000, 1000, 0);
public:
	TArray<FVector> PuntosRuta;

	int32 IndicePuntoRutaActual = 0;

	FVector PosicionInicial;
	float Tolerancia = 50.0f;

	float TiempoAcumulado = 0.0f;

public:
	bool bMovimientoAutonomo = false;

	FVector PosicionDestinoGameMode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Mover(float DeltaTime);

	virtual void CargarRuta();


private:


	UPROPERTY(VisibleAnywhere)

	UStaticMeshComponent* meshEnemy;

};