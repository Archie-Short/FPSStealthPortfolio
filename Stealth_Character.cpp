// Fill out your copyright notice in the Description page of Project Settings.


#include "Stealth_Character.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AStealth_Character::AStealth_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 64.0f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = false;

	StealthMovementComponent = CreateDefaultSubobject<UStealth_Movement_Component>(TEXT("Stealth Movement Component"));
	StealthMovementComponent->AttatchToPlayer(this); 
	

	

}

// Called to bind functionality to input
void AStealth_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AStealth_Character::LeftMousePressed);
	PlayerInputComponent->BindAction("LeftClick", IE_Released, this, &AStealth_Character::LeftMouseReleased);

	// Bind Lean events
	PlayerInputComponent->BindAction("Lean", IE_Pressed, this, &AStealth_Character::LeanPressed);
	PlayerInputComponent->BindAction("Lean", IE_Released, this, &AStealth_Character::LeanReleased);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AStealth_Character::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AStealth_Character::StopSprinting);

	
	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AStealth_Character::CheckJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AStealth_Character::StopJumping);
	

	
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this,  &AStealth_Character::CrouchInput);
	
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AStealth_Character::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AStealth_Character::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &AStealth_Character::VerticalCameraInput);

	
}

// Called when the game starts or when spawned
void AStealth_Character::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStealth_Character::StaticClass(), FoundActors);
	RaycastCollisionParams.AddIgnoredActors(FoundActors);
}

void AStealth_Character::MoveForward(float Val)
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->SetMoveForward(Val);
	}	
}

void AStealth_Character::MoveRight(float Val)
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->SetMoveRight(Val);
	}
}

void AStealth_Character::CrouchInput()
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->ToggleCrouch();
	}

}


void AStealth_Character::LeanPressed()
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->StartLeaning();
	}

}

void AStealth_Character::LeanReleased()
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->StopLeaning();
	}
}

void AStealth_Character::CheckJump() 
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->CanJump();
	}
}

void AStealth_Character::StartSprinting()
{
	if(StealthMovementComponent != nullptr)
	{StealthMovementComponent->StartSprinting(); }
	

}

void AStealth_Character::StopSprinting()
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->StopSprinting(); 
	}
}

void AStealth_Character::VerticalCameraInput(float Val)
{
	 if (Val){
		 const float currentPitch = FirstPersonCameraComponent->GetRelativeRotation().Pitch; 
	 	if(currentPitch > 65)
	 	{
	 		FirstPersonCameraComponent->SetRelativeRotation(FRotator(65.f,0.f,0.f));
	 	}
	 	else if(currentPitch < -65)
	 	{
	 		FirstPersonCameraComponent->SetRelativeRotation(FRotator(-65.f,0.f,0.f));

	 	}
	 	else if(currentPitch-Val <65 && currentPitch-Val > -65)
	 	{
	 		FirstPersonCameraComponent->AddLocalRotation(FRotator(-Val,0.f,0.f));

	 	}
	 }
}

void AStealth_Character::LeftMousePressed()
{
	bBlinkHeld = true; 
}

void AStealth_Character::LeftMouseReleased()
{
	bBlinkHeld = false;
	
}

// Called every frame
void AStealth_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if(bBlinkHeld)
	{
		FVector Forward = GetFirstPersonCameraComponent()->GetForwardVector();
		FVector Origin;
		FRotator Rotation;
		GetController()->GetPlayerViewPoint(Origin,Rotation); 
		FVector CheckLocation = Origin + (Forward * 200.0f);
		

		UWorld* const World = GetWorld();
		DrawDebugLine(GetWorld(), Origin, CheckLocation, FColor::Green, false,0.001f, 0, 2);

		FHitResult Hit; 
		if(GetWorld()->LineTraceSingleByChannel(Hit, Origin, CheckLocation, ECC_WorldStatic, RaycastCollisionParams))
		{
			FVector NewLocation = Hit.Location + (Hit.Normal*5);
			if (World != nullptr)
			{
				DrawDebugSphere(World,NewLocation,7.f,12,FColor::Yellow,false,-1,0,1.f); 
			}
		}
		else
		{
			if (World != nullptr)
			{
				DrawDebugSphere(World,CheckLocation,7.f,12,FColor::Blue,false,-1,0,1.f); 
			}
		}

	}
}
