// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Granade.generated.h"

UCLASS()
class ACTORPROJ_API AGranade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGranade();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void SlowTime(float TimeDilation);

	UFUNCTION()
		void RestoreTime();

	UFUNCTION()
		void RadialPush(float radio, float strength);

	FTimerHandle Timer, TimerOfTime;

	class UAudioComponent* ExplosionSound;

	class UParticleSystemComponent* Explosion;

	UFUNCTION()
		void GranadeExplosion();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Granade")
		class UStaticMeshComponent* ActorMesh;

	class USphereComponent* ActorCollider;
};
