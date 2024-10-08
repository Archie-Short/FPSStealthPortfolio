// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/GuardOverwatch.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AGuardOverwatch::AGuardOverwatch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGuardOverwatch::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if(World)
	{
		TArray<AActor*> FoundActors; 
		UGameplayStatics::GetAllActorsOfClass(World,AAIGuardController::StaticClass(),FoundActors);
		for(int i=0; i<FoundActors.Num(); i++)
		{
			AAIGuardController* Controller = Cast<AAIGuardController>(FoundActors[i]);
			Controller->AgentDeathDelegate.AddUniqueDynamic(this,&AGuardOverwatch::AddDeadGuardToWatch); 
			LevelAIControllers.Add(Controller); 
			
		}
		
	}
	
}

// Called every frame
void AGuardOverwatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGuardOverwatch::AddDeadGuardToWatch(AAICharacter* guard)
{
	for(auto* Controller: LevelAIControllers)
	{
		if(Controller->VisionComponent)
		{
			Controller->VisionComponent->AddToCharacterDetectionArray(guard);
			
		} 
	}
	
}

