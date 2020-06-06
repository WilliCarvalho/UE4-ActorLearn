// Fill out your copyright notice in the Description page of Project Settings.


#include "InsertingGranades.h"
#include "Granade.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"

// Sets default values
AInsertingGranades::AInsertingGranades()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ActorCollider = CreateDefaultSubobject<USphereComponent>(FName("Cllider"));
	RootComponent = ActorCollider;

}

// Called when the game starts or when spawned
void AInsertingGranades::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->SpawnActor<AGranade>(BpGranade, GetActorLocation(), GetActorRotation());
	
}

// Called every frame
void AInsertingGranades::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

