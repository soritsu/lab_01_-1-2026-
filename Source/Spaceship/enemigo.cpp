// Fill out your copyright notice in the Description page of Project Settings.


#include "enemigo.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
Aenemigo::Aenemigo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshEnemy = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshEnemy"));
    meshEnemy->SetSimulatePhysics(false);
	RootComponent = meshEnemy;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshEnemyAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (MeshEnemyAsset.Succeeded())
	{
		meshEnemy->SetStaticMesh(MeshEnemyAsset.Object);
	}

	speed = 1000.0f;
	indiceRuta = 0;
}

// Called when the game starts or when spawned
void Aenemigo::BeginPlay()
{
	Super::BeginPlay();

	CargarRuta();
	
}

// Called every frame
void Aenemigo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Mover(DeltaTime);
}
void Aenemigo::CargarRuta() {
	
	FVector inicio = GetActorLocation();

	ruta.Add(inicio);
	ruta.Add(inicio + FVector(0, 500, 0));
	ruta.Add(inicio + FVector(500, 500, 0));
	ruta.Add(inicio + FVector(500, 0, 0));
}
void Aenemigo::Mover(float DeltaTime) {
	
	FVector pocionActual = GetActorLocation();
	FVector destino = ruta[indiceRuta];

	FVector nuevapocion = FMath::VInterpConstantTo(
		pocionActual, 
		destino, 
		DeltaTime,
		speed);
	SetActorLocation(nuevapocion);
	if (pocionActual.Equals(destino, 1.0f)) {
		indiceRuta++;
		if (indiceRuta >= ruta.Num()) {
			indiceRuta = 0;
		}
	}

}