// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "Engine\TargetPoint.h"
#include "Kismet\GameplayStatics.h"
#include "Granade.h"
#include "Engine\World.h"
#include "Engine\Engine.h"
#include "UObject\ConstructorHelpers.h"


AMyGameMode::AMyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn>PlayerPawnBPClass(TEXT("Blueprint'/Game/ThirdPersonBP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter_C'"));

	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMyGameMode::StartPlay()
{
	Super::StartPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("I'm in Game Mode \n Granades Launched"));
}

//Game mode é uma classe que contém as regras e cnfigurações do nosso jogo
//Cada level pode ter seu próprio game mode
//para isso, em World Settings você pode sobrescrever o gamemode padrão pelo seu gamemode
//caso deseje substituir o gamemode para todos os levels use Edit->Project Settings
void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	//LocalGranades Array has an error that it only has items on Odd array positions (ex.: 1, 3, 5, 7...)
	TArray<AActor*> LocalGranades;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), LocalGranades);
	UE_LOG(LogTemp, Warning, TEXT("Num de target Point encontrados %d"), LocalGranades.Num());

	if (LocalGranades.Num() != 0)
	{
		FActorSpawnParameters params = FActorSpawnParameters();
		//Indicate where the LocalGranades array item is and show it's posisiotn
		int32 i = 0;
		for (auto It : LocalGranades)
		{
			GetWorld()->SpawnActor<AGranade>(It->GetActorLocation(), It->GetActorRotation(), params);
			UE_LOG(LogTemp, Warning, TEXT("What is happening: %p in LocalGranades in the %d Index"), It, i++);
			//GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("Actor generated"));
			//UE_LOG(LogTemp, Warning, TEXT("Transform = %s"), *It->GetActorLocation().ToString());
		}
	}
}

