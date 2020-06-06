// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InsertingGranades.generated.h"

UCLASS()
class ACTORPROJ_API AInsertingGranades : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInsertingGranades();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class USphereComponent* ActorCollider;

	UPROPERTY(EditAnyWhere, Category = "Granade Type")
		TSubclassOf<class AGranade> BpGranade;

};
