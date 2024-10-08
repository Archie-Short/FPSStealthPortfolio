// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <vector>

#include "PlayerLogic/PlayerCharacter.h"
#include "AI_VisionComponent.generated.h"

class AAICharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAwarenessChangedSignature, float, AwarenessUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerVisibleSignature,bool, bIsPlayerVisible,FVector,LastLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBodySeenSignature, FVector, DangerSource); 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INDIVIDUALPROJECT_API UAI_VisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAI_VisionComponent();

	void LinkToAI(AAICharacter* AI); 

	//Focused Cone Variables
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Vision Cones")
	float FocVisLength;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Vision Cones")
	float FocVisHeight;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Vision Cones")
	float FocVisWidth;
	
	//Peripheral Cone Variables
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Vision Cones")
	float PeriphVisLength;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Vision Cones")
	float PeriphVisHeight;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Vision Cones")
	float PeriphVisWidth;


	//Detection Values
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Vision Data")
	float FocusDetectionRate;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Vision Data")
	float PeriphDetectionRate;


	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category="Vision Data")
	float Awareness; 

	UPROPERTY(BlueprintAssignable)
	FOnAwarenessChangedSignature OnAwarenessChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerVisibleSignature OnPlayerVisibleDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnBodySeenSignature OnBodySeenDelegate; 

	//RaycastParameters
	FCollisionQueryParams TraceParams; 
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void DrawDebugCones(float dt);

	float VisibilityScore(FTransform* HeadTransform, FVector ActorLocation, bool bIsPlayer = false);

	

	//AI Skeletal Mesh
	UPROPERTY(BlueprintReadOnly, Category="Vision Data")
	USkeletalMeshComponent* AIMesh;
	
	//AI Actor 
	UPROPERTY(BlueprintReadOnly, Category="Vision Data")
	AAICharacter* AIActor;
	
	//Actors to detect
	UPROPERTY(BlueprintReadWrite, Category="Vision Data")
	TArray<AActor*> ActorsToDetect;

	//Actors to detect
	UPROPERTY(BlueprintReadWrite, Category="Vision Data")
	TArray<ACharacter*> CharactersToDetect;

	//Player to detect
	UPROPERTY(BlueprintReadWrite, Category="Vision Data")
	APlayerCharacter* Player; 

	//Eye Location and Direction
	FVector EyeDirection;
	FVector EyeLocation;

	void UpdateAwareness(float ChangeValue, float DT);

	


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddToObjectDetectionArray(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void AddToCharacterDetectionArray(ACharacter* Actor);
	
};
