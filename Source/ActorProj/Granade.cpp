// Fill out your copyright notice in the Description page of Project Settings.


#include "Granade.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/EngineTypes.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Public/EngineUtils.h"
#include "UObject/UObjectIterator.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/SkeletalMeshActor.h"
#include "UObject/UObjectBaseUtility.h"
#include "MyPlayer.h"

// Sets default values
AGranade::AGranade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActorCollider = CreateDefaultSubobject<USphereComponent>(FName("Collider"));
	ActorCollider->SetSphereRadius(12.f);
	ActorCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = ActorCollider;

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ActorMesh"));
	ActorMesh->SetRelativeLocation(FVector(3.2f, 0.f, 0.f));
	ActorMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	ActorMesh->SetupAttachment(RootComponent);

	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(FName("Explosion Sound"));
	ExplosionSound->bAutoActivate = false;
	ExplosionSound->bAutoDestroy = true;
	ExplosionSound->SetupAttachment(ActorMesh);

	Explosion = CreateDefaultSubobject<UParticleSystemComponent>(FName("Explosion Particle"));
	Explosion->bAutoActivate = false;
	Explosion->bAutoDestroy = true;
	Explosion->SetupAttachment(ActorMesh);


	ConstructorHelpers::FObjectFinder<USoundCue>GranadeSound(TEXT("SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>Granade(TEXT("StaticMesh'/Game/Fbx/granada.granada'"));

	ConstructorHelpers::FObjectFinder<UParticleSystem>ExplosionParticle(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));

	if (Granade.Succeeded() && GranadeSound.Succeeded() && ExplosionParticle.Succeeded()) {
		ActorMesh->SetStaticMesh(Granade.Object);
		ExplosionSound->SetSound(GranadeSound.Object);
		Explosion->SetTemplate(ExplosionParticle.Object);
	}
}

void AGranade::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Granade Destroyed!");
	GetWorldTimerManager().ClearTimer(Timer);
}

// Called when the game starts or when spawned
void AGranade::BeginPlay()
{
	Super::BeginPlay();
	SlowTime(0.1);
	ActorMesh->SetSimulatePhysics(true);
	ActorMesh->SetEnableGravity(true);
	GetWorldTimerManager().SetTimer(Timer, this, &AGranade::GranadeExplosion, 5.f, false);
}

void AGranade::SlowTime(float TimeDilation)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
	GetWorldTimerManager().SetTimer(TimerOfTime, this, &AGranade::RestoreTime, (3.f * TimeDilation), false);
}

void AGranade::RestoreTime()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	GetWorldTimerManager().ClearTimer(TimerOfTime);
}

void AGranade::RadialPush(float radio, float strength)
{
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsA(AStaticMeshActor::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Name: %s %X"), *ActorItr->GetName(), *ActorItr);
			UStaticMeshComponent* StaticMeshRoot;
			StaticMeshRoot = Cast<UStaticMeshComponent>(ActorItr->GetRootComponent());
			if (StaticMeshRoot)
			{
				if (StaticMeshRoot->Mobility == EComponentMobility::Movable && (ActorItr->ActorHasTag(FName("RightChair"))))
				{
					StaticMeshRoot->SetSimulatePhysics(true);
					StaticMeshRoot->AddRadialImpulse(GetActorLocation(), radio, strength, ERadialImpulseFalloff::RIF_Linear, true);
				}
			}
		}
		else if (ActorItr->IsA(ASkeletalMeshActor::StaticClass()) || ActorItr->ActorHasTag("White"))
		{
			UE_LOG(LogTemp, Error, TEXT("Skeletal Name: %s"), *ActorItr->GetName());
		}
	}

	for (TObjectIterator<USkeletalMeshComponent> It; It; ++It)
	{
		if (It->GetWorld() == GetWorld())
		{
			if (It->ComponentHasTag(FName("PlayerCPP")) ) 
			{
				UE_LOG(LogTemp, Warning, TEXT("Name: %s End: %X \n"), *It->GetName(), *It);
				ACharacter* OtherEnemy = Cast<ACharacter>(It->GetOwner());
				UParticleSystemComponent* Particle = Cast<UParticleSystemComponent>(OtherEnemy->GetComponentByClass(UParticleSystemComponent::StaticClass()));
				Particle->Activate(true);
				It->SetSimulatePhysics(true);
				It->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				It->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
				It->AddRadialImpulse(GetActorLocation(), radio, strength, ERadialImpulseFalloff::RIF_Linear, true);
			}
		}
	}
}

void AGranade::GranadeExplosion()
{
	SetLifeSpan(5.5f);
	ExplosionSound->Play();
	SlowTime(0.05);
	Explosion->ActivateSystem();
	ActorMesh->SetVisibility(false);
	RadialPush(5000.f, 1000.f);
}

// Called every frame
void AGranade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

