// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/PatrolRoute.h"
#include "Components/SplineComponent.h"

APatrolRoute::APatrolRoute()
{
	PrimaryActorTick.bCanEverTick = false;

	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	Path->SetupAttachment(RootComponent);
	Path->bDrawDebug = true; 
}

void APatrolRoute::BeginPlay()
{
	Super::BeginPlay();
	GetSplinePoints();
}

void APatrolRoute::GetSplinePoints()
{
	for(int i=0; i<= Path->GetNumberOfSplinePoints(); i++)
	{
		Locations.Add(Path->GetLocationAtSplinePoint(i,ESplineCoordinateSpace::World)); 
	}
}


