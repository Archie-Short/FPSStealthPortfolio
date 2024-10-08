// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementStates.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "PlayerCharacter_MoveComponent.generated.h"

class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INDIVIDUALPROJECT_API UPlayerCharacter_MoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerCharacter_MoveComponent();


protected:

	//Sounds Generation
	//Hearing
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= "Sound")
	UAIPerceptionStimuliSourceComponent* FootstepStimulus;

	
	//Variables//

	FCollisionQueryParams RaycastCollisionParams;
	TArray<TEnumAsByte<EObjectTypeQuery>> SphereTraceObjectTypes;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bShowDebug = true; 

	
	//ENUMS 
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Movement")
	TEnumAsByte<EStance> CharacterStance = EStanceStanding;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Movement")
	TEnumAsByte<EMovementAbility> CharacterMovementAbility = EmaNeutral;  
	
	//Input MovementVector
	FVector2d MovementVector = FVector2d(0.f,0.f); 
	
	//Crouching Variables 
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Crouching")
	float CapsuleScaleMin = .6f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Crouching")
	float CapsuleHeightMin = 68.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Crouching")
	float CapsuleScaleChangeRate = 1.8f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Crouching")
	float CrouchedMovementRate = .5f;

	//Headbobbing Modifiers

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Headbobbing")
	bool bHeadBobbingEnabled = true;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Headbobbing")
	float HeadBobbingMaxHeight = 10.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Headbobbing")
	float HeadBobbingMinHeight = -10.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Headbobbing")
	float HeadBobbingDefaultHeight = 64.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Headbobbing")
	float HeadBobbingRate = .8f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Headbobbing")
	bool bHeadBobbingUp = true;

	//Leaning 

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Leaning")
	float LeanRate = 10.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Leaning")
	float LeanDistance = 90.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Leaning")
	float LeanAngle = 10.f;
	
	FVector2D LeanTarget = FVector2D(0.f,64.f);
	FVector CameraNeutralLean;
	float LastBobbingHeight = HeadBobbingDefaultHeight;

	//Stooping Variables

	FVector StoopTarget = FVector(0.f,0.f,0.f);
	FVector RiseTarget =  FVector(0.f,0.f,0.f); 

	UCapsuleComponent*  CharacterCapsuleStanding; 
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Stooping")
	float StoopCastDistance = 95.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Stooping")
	float StoopMinScale = .3f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Stooping")
	float StoopRate = 1.5f;
	
	
	//Vaulting Variables
	
	EVaultingState VaultingState = Vaulst_Not_Vaulting;  
	FVector Ending_Location;
	FVector Starting_Location; 

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Vaulting")
	float MinVaultHeight = 40.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Vaulting")
	float MaxVaultHeight = 170.f;

	float VaultingProgress = 0.f;
	
	//FUNCTIONS
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 

	//Crouching Functions
	void Crouch(float DeltaTime);
	void UnCrouch(float DeltaTime);

	//Leaning Functions 
	void LeanCheckAndMove(float DeltaTime) const;
	void ReturnFromLean(float DeltaTime);
	
	//Stoop Function
	void CanStoop(bool& Result, FVector& ObjectPosition);
	bool StoopedToTarget(float DeltaTime);
	bool RiseToTarget(float DeltaTime);
	
	//Gets Different locations around hte player capsule
	FVector GetTopOfPlayerScaled();
	FVector GetTopOfPlayerStanding(); 
	FVector GetBottomOfPlayer(); 
	FVector GetObjectAbovePlayer(bool &ObjectStraightAbove);
	
	//Movement
	void Move(); 
	void Step();
	void SpeedCheck(); 

	//Vaulting

	void CanVault(bool& Result, FVector& EndingLocation);
	void CanVaultToHit(FHitResult VaultHit,bool& Result, FVector& EndingLocation);
	bool CheckCapsuleCollision(FVector Center, float HalfHeight, float Radius, bool DrawDebug);
	void Vault();
	void TickVault(float DeltaTime);
	

public:	
	void AttatchToPlayer(APlayerCharacter* CharacterPtr); 
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Inputs 
	void ToggleCrouch();
	
	void StartLeaning();
	void StopLeaning();

	void StartSprinting();
	void StopSprinting(); 

	void SetMoveForward(float Val);
	void SetMoveRight(float Val);

	void CanJump(); 


	//UI

	/*
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMovementDebugHUD> MovementDebugHUDClass;

	UPROPERTY()
	class UMovementDebugHUD* MovementDebugHUD;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAbilityPopupHUD> AbilityPopupHUDClass;

	UPROPERTY()
	class UAbilityPopupHUD* AbilityPopupHUD; 
*/ 
private:
	//Reference to the player 
	APlayerCharacter* Character;

	
	
};