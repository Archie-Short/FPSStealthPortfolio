// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/AI_InformationComponent.h"
#include "Engine/DataTable.h"
#include <string>

// Sets default values for this component's properties
UAI_InformationComponent::UAI_InformationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	
}


// Called when the game starts
void UAI_InformationComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

	if(bGenerateBlankInformation && InformationTable)
	{
		GenerateAIInformation(); 
	}
}

void UAI_InformationComponent::GenerateAIInformation()
{
	const FString ContextString = FString(TEXT("Debugging Information Table"));

	//Generate First Name and Gender
	if(AIInformation.AIFirstName.IsEmpty())
	{
		int Row = FMath::RandRange(0, 200);
		FString RowString = FString::FromInt(Row);
		FName RowName  = FName(FString::FromInt(Row)); 
		FAIInformationRow* InformationRow = InformationTable->FindRow<FAIInformationRow>(RowName,ContextString);
		if(InformationRow)
		{
			AIInformation.AIFirstName = InformationRow->FirstName;
			AIInformation.AIGender = InformationRow->Gender; 
		}
			
	}
	//Generate Last Name
	if(AIInformation.AILastName.IsEmpty())
	{
		int Row = FMath::RandRange(0, 200);
    	FString RowString = FString::FromInt(Row);
		FName RowName  = FName(FString::FromInt(Row)); 
		FAIInformationRow* InformationRow = InformationTable->FindRow<FAIInformationRow>(RowName,ContextString);
		if(InformationRow)
		{
			AIInformation.AILastName = InformationRow->LastName; 
    	}
	}

	//Generate Eye Colour
	if(AIInformation.AIEyeColour.IsEmpty())
	{
		int Row = FMath::RandRange(0, 54);
		FString RowString = FString::FromInt(Row);
		FName RowName  = FName(FString::FromInt(Row)); 
		FAIInformationRow* InformationRow = InformationTable->FindRow<FAIInformationRow>(RowName,ContextString);
		if(InformationRow)
		{
			AIInformation.AIEyeColour = InformationRow->EyeColour; 
		}
	}

	//Generate Hair Colour
	if(AIInformation.AIHairColour.IsEmpty())
	{
		int Row = FMath::RandRange(0, 6);
		FString RowString = FString::FromInt(Row);
		FName RowName  = FName(FString::FromInt(Row)); 
		FAIInformationRow* InformationRow = InformationTable->FindRow<FAIInformationRow>(RowName,ContextString);
		if(InformationRow)
		{
			AIInformation.AIHairColour = InformationRow->HairColour; 
		}
	}

	//Generate Age
	if(AIInformation.AIAge == 0)
	{
		int Age = FMath::RandRange(23, 48);
		AIInformation.AIAge = Age; 
	}
	UE_LOG(LogTemp,Warning,TEXT("First Name: %s"),*AIInformation.AIFirstName);
	UE_LOG(LogTemp,Warning,TEXT("Last Name: %s"),*AIInformation.AILastName); 
	UE_LOG(LogTemp,Warning,TEXT("Gender: %s"),*AIInformation.AIGender); 
	UE_LOG(LogTemp,Warning,TEXT("Age: %d"),AIInformation.AIAge); 
	UE_LOG(LogTemp,Warning,TEXT("Eye Colour: %s"),*AIInformation.AIEyeColour);
	UE_LOG(LogTemp,Warning,TEXT("Hair Colour: %s"),*AIInformation.AIHairColour); 
}


// Called every frame
void UAI_InformationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

