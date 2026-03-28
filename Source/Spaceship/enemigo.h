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
	
public:
	TArray<FVector> ruta;
	int indiceRuta;

	float speed;
	

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