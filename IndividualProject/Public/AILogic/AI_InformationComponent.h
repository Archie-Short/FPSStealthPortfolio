// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Traits/PerceptionTraitBase.h"
#include "AI_InformationComponent.generated.h"

USTRUCT(BlueprintType)
struct FAIInformation
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString AIFirstName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString AILastName;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString AIHairColour;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString AIEyeColour;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString AIGender;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int AIAge;
};

USTRUCT(BlueprintType)
struct FAIInformationRow : public FTableRowBase
{
	GENERATED_BODY();

public: 
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString FirstName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString Gender;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString LastName;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString EyeColour;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString HairColour;

	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INDIVIDUALPROJECT_API UAI_InformationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAI_InformationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void GenerateAIInformation(); 
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FAIInformation AIInformation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UDataTable* InformationTable; 

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bGenerateBlankInformation;

	//Traits
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UPerceptionTraitBase> PerceptionTraitClass; 

	//Traits
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UPerceptionTraitBase* PerceptionTrait; 

		
};
