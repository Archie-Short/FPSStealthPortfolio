// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerLogic/PlayerCharacter.h"

#include "InterchangeResult.h"
#include "AILogic/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
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

	StealthMovementComponent = CreateDefaultSubobject<UPlayerCharacter_MoveComponent>(TEXT("Stealth Movement Component"));
	StealthMovementComponent->AttatchToPlayer(this);

	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Hearing>());
	stimulus->RegisterWithPerceptionSystem(); 
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	TraceParams = FCollisionQueryParams(FName(TEXT("Line Trace")),true);
	TraceParams.AddIgnoredActor(this); 

	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Check for backstab
	FHitResult Hit; 
	UWorld* World = GetWorld();

	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * 2000);
	
	if(IsValid(World))
	{
		if(World->LineTraceSingleByChannel(Hit, Start,End,ECollisionChannel::ECC_Camera,TraceParams))
		{
			//Check if uses interface
			if(Hit.GetActor()->Implements<UEnemy>())
			{

				if(!bEnemyInView)
				{
					LookAtEnemy = Hit.GetActor();
					bEnemyInView = true; 
					UpdateEnemyInView(bEnemyInView); 
				}

				if(FVector::Distance(Start,Hit.Location) < 200){
					float AngleBehind = IEnemy::Execute_AngleFromFacing(Hit.GetActor(), GetActorLocation());
					if(AngleBehind >= 25.f)
					{
						if(!bCanBackStab)
						{
							bCanBackStab = true;
							UpdateCanBackstab(true);
						
						}
						BackstabLocation = Hit.ImpactPoint; 

					}
					else
					{
						if(bCanBackStab)
						{
							bCanBackStab = false; UpdateCanBackstab(false);
						}
					}
				}
			}


			else
			{

				if(bEnemyInView)
				{
					LookAtEnemy = nullptr;
					bEnemyInView = false; 
					UpdateEnemyInView(bEnemyInView); 
				}
			
				if(bCanBackStab)
				{
					bCanBackStab = false;UpdateCanBackstab(false);
				}
			}
			}
		
	}

	







	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Bind Lean events
	PlayerInputComponent->BindAction("Lean", IE_Pressed, this, &APlayerCharacter::LeanPressed);
	PlayerInputComponent->BindAction("Lean", IE_Released, this, &APlayerCharacter::LeanReleased);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprinting);

	
	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::CheckJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);
	
	
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this,  &APlayerCharacter::CrouchInput);
	
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &APlayerCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APlayerCharacter::VerticalCameraInput);

	//Attack 
	//PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &APlayerCharacter::PrimaryActionPressed);

	PlayerInputComponent->BindAction("Attention", IE_Pressed,this,&APlayerCharacter::Distract);
}


void APlayerCharacter::PrimaryActionPressed()
{
	//Perform Backstab 
	if(bCanBackStab)
	{
		if(IsValid(LookAtEnemy))
		{
			if(LookAtEnemy->Implements<UEnemy>())
			{
				IEnemy::Execute_Die(LookAtEnemy);
			}
		}
	}
}

void APlayerCharacter::Distract()
{
	UWorld* World = GetWorld();
	if(World)
	{
		if(Clap)
		{
			FVector Location = GetActorLocation();
			UGameplayStatics::PlaySoundAtLocation(World,Clap,Location);
			UAISense_Hearing::ReportNoiseEvent(World,Location,1.0,this,500,TEXT("Noise"));
			
		
		}
	}
}

void APlayerCharacter::MoveForward(float Val)
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->SetMoveForward(Val);
	}	
}

void APlayerCharacter::MoveRight(float Val)
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->SetMoveRight(Val);
	}
}

void APlayerCharacter::CrouchInput()
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->ToggleCrouch();

		BlueprintCrouchToggle(); 
		
	}

}




void APlayerCharacter::LeanPressed()
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->StartLeaning();
	}

}

void APlayerCharacter::LeanReleased()
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->StopLeaning();
	}
}

void APlayerCharacter::CheckJump() 
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->CanJump();
	}
}

void APlayerCharacter::StartSprinting()
{
	if(StealthMovementComponent != nullptr)
	{StealthMovementComponent->StartSprinting(); }
	

}

void APlayerCharacter::StopSprinting()
{
	if(StealthMovementComponent != nullptr)
	{
		StealthMovementComponent->StopSprinting(); 
	}
}

void APlayerCharacter::VerticalCameraInput(float Val)
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