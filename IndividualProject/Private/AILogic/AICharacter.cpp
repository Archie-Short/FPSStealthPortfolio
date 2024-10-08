// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/AICharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GuardAwareness=0;
	IsAlive = true;

	AIInformationComponent = CreateDefaultSubobject<UAI_InformationComponent>("Information Component");
	
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAICharacter::UpdateAwareness(float AwarenessUpdate)
{
	GuardAwareness = AwarenessUpdate; 
}

void AAICharacter::SetMovementSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed; 
}

void AAICharacter::Die_Implementation()
{
	IsAlive = false;
	GuardAwareness = 0; 
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->DestroyComponent(); 
	OnDeathDelegate.Broadcast(); 
}

float AAICharacter::AngleFromFacing_Implementation(FVector PlayerLocation)
{
	FVector ToPlayer = (GetActorLocation() - PlayerLocation).GetUnsafeNormal(); 
	float Pitch = FMath::RadiansToDegrees(FVector::DotProduct(GetActorForwardVector(),ToPlayer)); 
	return Pitch;
}

bool AAICharacter::GetDeathState_Implementation()
{
	return IsAlive; 
}

UAI_InformationComponent* AAICharacter::GetAIInformation_Implementation()
{
	return AIInformationComponent; 
}

void AAICharacter::Bark_Implementation(const FString &ToSay)
{
	//IEnemy::Bark_Implementation(ToSay);
}


// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

