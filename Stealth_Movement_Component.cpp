// Fill out your copyright notice in the Description page of Project Settings.

#include "Stealth_Movement_Component.h"
#include "Stealth_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MovementDebugHUD.h"
#include "AbilityPopupHUD.h" 
#include "Blueprint/UserWidget.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values for this component's properties
UStealth_Movement_Component::UStealth_Movement_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MovementDebugHUDClass = nullptr;
	MovementDebugHUD = nullptr; 
}


// Called when the game starts
void UStealth_Movement_Component::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStealth_Character::StaticClass(), FoundActors);
	RaycastCollisionParams.AddIgnoredActors(FoundActors);

	
	SphereTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	SphereTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	SphereTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	if(Character != nullptr)
		{
		CameraNeutralLean = Character->GetFirstPersonCameraComponent()->GetRelativeLocation();
		}

	if(bShowDebug)
	{
		if(MovementDebugHUDClass)
		{
			MovementDebugHUD = Cast<UMovementDebugHUD>(CreateWidget(GetWorld(),MovementDebugHUDClass)); 
			MovementDebugHUD->AddToPlayerScreen(); 
		}
	}

	if(AbilityPopupHUDClass)
	{
		AbilityPopupHUD = Cast<UAbilityPopupHUD>(CreateWidget(GetWorld(),AbilityPopupHUDClass));
		AbilityPopupHUD->SetAbilityText(TEXT("Press SPACE to vault"));
	}
}

void UStealth_Movement_Component::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(MovementDebugHUD)
	{
		MovementDebugHUD->RemoveFromParent();
		MovementDebugHUD = nullptr; 
	}

	if(AbilityPopupHUD)
	{
		AbilityPopupHUD->RemoveFromParent();
		AbilityPopupHUD = nullptr; 
	}
}


void UStealth_Movement_Component::ToggleCrouch()
{
	//**if stooping do nothing**//
	if(CharacterStance == EStanceStanding)
	{
		if(Character->GetCapsuleComponent()->GetComponentScale().Z >= 0.98)
		{
			CharacterCapsuleStanding = Character->GetCapsuleComponent();
		}
		CharacterStance = EStanceAttemptingCrouch; 

	}
	else if(CharacterStance == EStanceCrouched)
	{
		CharacterStance = EStanceAttemptingStand; 
	}
}

void UStealth_Movement_Component::Move()
{
	if(Character != nullptr)
	{
		//If the character is leaning set the leaning target 
		if(CharacterMovementAbility == EmaLeaning)
		{
			LeanTarget = FVector2d( CameraNeutralLean.Y+(MovementVector.X * LeanDistance),CameraNeutralLean.Z+(MovementVector.Y*(LeanDistance/2))); 
		}

		//Ignore if no input is given 
		else if(MovementVector.Length() != 0.f)
		{

			SpeedCheck();

			float SpeedModifier = 1.f;

			if(CharacterMovementAbility == EmaSprinting)
			{
				SpeedModifier = 1.5f; 
			}
			//**Add In Sprint, crouch and stoop movement speed modifiers** //
		
			Character->AddMovementInput(Character->GetActorForwardVector(), MovementVector.Y*SpeedModifier);

			Character->AddMovementInput(Character->GetActorRightVector(), MovementVector.X*SpeedModifier);


		
			if(bHeadBobbingEnabled&& CharacterMovementAbility != EmaReturningFromLean && CharacterStance != EStanceAttemptingStoop){
				float HeadBobbingRateModifier = 1.f;

				//Crouched Sprinting Modifer
				if(CharacterMovementAbility == EmaSprinting && CharacterStance == EStanceCrouched)
				{
					HeadBobbingRateModifier = 1.05f;
				}
				//Sprinting Modifer
				if(CharacterMovementAbility == EmaSprinting)
				{
					HeadBobbingRateModifier = 0.9f;

				}
				//Crouching Modifer
				if(CharacterStance == EStanceCrouched)
				{
					HeadBobbingRateModifier = 1.1f;

				}
				

				//**Add in check for leaning an stooping not to headbob **// 
				if(!Character->GetMovementComponent()->IsFalling())
				{
					FVector CurrentCameraPosition = Character->GetFirstPersonCameraComponent()->GetRelativeLocation();

					//Headbobbing
					if(bHeadBobbingUp)
					{
						if(CurrentCameraPosition.Z >= HeadBobbingDefaultHeight+HeadBobbingMaxHeight)
						{
							bHeadBobbingUp = false;
						}
						else
						{
							CurrentCameraPosition += FVector(0.f,0.f,HeadBobbingRate*HeadBobbingRateModifier);
						}
					}
					else
					{
						if(CurrentCameraPosition.Z <= HeadBobbingDefaultHeight+HeadBobbingMinHeight)
						{
							bHeadBobbingUp = true;
							Step(); 
						}
						else
						{
							CurrentCameraPosition -= FVector(0.f,0.f,HeadBobbingRate*HeadBobbingRateModifier);

						}
					}
					Character->GetFirstPersonCameraComponent()->SetRelativeLocation(CurrentCameraPosition);
				}
		
			}
		}}
}

void UStealth_Movement_Component::Step()
{
// 	UE_LOG(LogTemp,Warning,TEXT("Step Taken")); 
}

void UStealth_Movement_Component::SpeedCheck()
{
	if(CharacterStance == EStanceStooped || CharacterStance == EStanceCrouched || CharacterStance == EStanceAttemptingCrouch)
	{
		//Max Speed, Speed modifier
		Character->GetCharacterMovement()->MaxWalkSpeed = 200.f; 
	}
	else if(CharacterStance == EStanceStanding)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 400.f; 

		
	}

	if(CharacterMovementAbility == EmaSprinting)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 550.f; 

	}
	
}

void UStealth_Movement_Component::CanJump()
{
	if(Character != nullptr)
	{
		if(CharacterStance <= 3)
		{

			if(VaultingState == Vaulst_Can_Vault)
			{
				Vault();
				AbilityPopupHUD->RemoveFromParent(); 

			}
			else
			{
				Character->Jump();

			}
		}
	}
}

void UStealth_Movement_Component::CanVault(bool& Result, FVector& EndingLocation)
{
	if(Character != nullptr)
	{
		if(VaultingState != Vaulst_Vaulting)
		{
			FVector CharacterLocation = Character->GetCapsuleComponent()->GetComponentLocation();
			FVector ForwardVector = Character->GetFirstPersonCameraComponent()->GetForwardVector() * FVector(70.f,70.f,1.f) ;
			FVector StartingPoint = CharacterLocation+ForwardVector+FVector(0.f,0.f,130.f);
			FVector EndingPoint = StartingPoint - FVector(0.f,0.f,220.f); 
			FHitResult Hit;

		//	DrawDebugLine(GetWorld(), StartingPoint, EndingPoint, FColor::Red, false,0.001f, 0, 2);

			if(GetWorld()->LineTraceSingleByChannel(Hit, StartingPoint, EndingPoint, ECC_WorldStatic, RaycastCollisionParams))
			{
				FVector VaultingToPosition = FVector(0.f,0.f,0.f); 
				CanVaultToHit(Hit, Result, VaultingToPosition);

				if(Result)
				{
					//UE_LOG(LogTemp,Warning,TEXT("Can Vault"));
					EndingLocation = VaultingToPosition;
				}
			}

		}
		else
		{
			Result = false;
			Ending_Location = FVector(0.f,0.f,0.f);
		}
	}
}

void UStealth_Movement_Component::CanVaultToHit(FHitResult VaultHit, bool& Result, FVector& EndingLocation)
{
	//Ledge is appropriate Height
	float HeightFromFeet = VaultHit.Location.Z - VaultHit.TraceEnd.Z;

	if(!(HeightFromFeet > MinVaultHeight && HeightFromFeet < MaxVaultHeight))
	{
		Result = false;
		EndingLocation = FVector(0.f,0.f,0.f);
		return; 
	}
	
	//UE_LOG(LogTemp,Warning,TEXT("Height From Feet: %f"),HeightFromFeet);
	//UE_LOG(LogTemp,Warning,TEXT("Angle Of Object: %f"),VaultHit.Normal.Z);

	//Surface is walkable (Checking the angle)
	if(VaultHit.Normal.Z < Character->GetCharacterMovement()->K2_GetWalkableFloorZ())
	{
		Result = false;
		EndingLocation = FVector(0.f,0.f,0.f);
		return; 
	}
	
	//Room for us to fit in
	UCapsuleComponent* CharacterCapsule = Character->GetCapsuleComponent(); 
	EndingLocation = VaultHit.Location + FVector(0.f,0.f,CharacterCapsule->GetScaledCapsuleHalfHeight());
	FVector ToCheck = EndingLocation + FVector(0.f,0.f,CharacterCapsule->GetScaledCapsuleRadius());
	
	if(!(CheckCapsuleCollision(ToCheck,CharacterCapsule->GetScaledCapsuleHalfHeight(),CharacterCapsule->GetScaledCapsuleRadius(),true)))
	{
		Result = false;
		EndingLocation = FVector(0.f,0.f,0.f);
		return; 
	}
	
	//Return
	Result = true;
}

bool UStealth_Movement_Component::CheckCapsuleCollision(FVector Center, float HalfHeight, float Radius, bool DrawDebug)
{
	UWorld* const World = GetWorld();
	UCapsuleComponent* CharacterCapsule = Character->GetCapsuleComponent(); 

	TArray<AActor*> EmptyArray1;
	TArray<AActor*> EmptyArray2;

	if(UKismetSystemLibrary::CapsuleOverlapActors(World,Center,Radius,HalfHeight,SphereTraceObjectTypes,nullptr,EmptyArray1,EmptyArray2))
	{
		if(DrawDebug)
		{if (World != nullptr)
		{
			if(bShowDebug)
			{
				DrawDebugCapsule(World,Center,HalfHeight,Radius,CharacterCapsule->GetComponentQuat(),FColor::Red,false,-1,0,2.f);
			}
		}}
		return false;
	}
	if(DrawDebug)
	{if (World != nullptr)
	{
		if(bShowDebug)
		{
			DrawDebugCapsule(World,Center,HalfHeight,Radius,CharacterCapsule->GetComponentQuat(),FColor::Green,false,-1,0,2.f);
		}
	}}
	 
	return true; 
}

void UStealth_Movement_Component::Vault()
{
	VaultingProgress = 0.f;
	Starting_Location = Character->GetActorLocation();
	VaultingState = Vaulst_Vaulting;
}

void UStealth_Movement_Component::TickVault(float DeltaTime)
{
	
	if(VaultingProgress <=.6f)
	{
		VaultingProgress += FMath::Square(DeltaTime/.3f)+DeltaTime/.3f;

		FVector CurrentPosition = FMath::Lerp(Starting_Location,Ending_Location,VaultingProgress);
		Character->GetCapsuleComponent()->SetWorldLocation(CurrentPosition);
	}

	if(VaultingProgress <=1.0f)
	{
		VaultingProgress += DeltaTime/.7f;

		FVector CurrentPosition = FMath::Lerp(Starting_Location,Ending_Location,VaultingProgress);
		Character->GetCapsuleComponent()->SetWorldLocation(CurrentPosition);
	}
	else
	{
		VaultingState = Vaulst_Not_Vaulting; 
	}
	
}



void UStealth_Movement_Component::SetMoveForward(float Val)
{
	MovementVector.Y = Val; 
}

void UStealth_Movement_Component::SetMoveRight(float Val)
{
	MovementVector.X = Val; 

}


void UStealth_Movement_Component::Crouch(const float DeltaTime)
{
	if(Character != nullptr)
	{
		UCapsuleComponent* Capsule = Character->GetCapsuleComponent(); 
		FVector CapsuleScale = Capsule->GetComponentScale();
		
		//Lower the capsule scale in the Z axis to make the character smaller first
		if(CapsuleScale.Z > CapsuleScaleMin)
		{
			CapsuleScale.Z -= DeltaTime * CapsuleScaleChangeRate;
			Capsule->SetWorldScale3D(CapsuleScale);
		}

		else
		{
			CapsuleScale.Z = CapsuleScaleMin;
			Capsule->SetWorldScale3D(CapsuleScale);
			CharacterStance = EStanceCrouched; 
		}
	}
}

void UStealth_Movement_Component::UnCrouch(const float DeltaTime)
{
	if(Character != nullptr)
	{
		UCapsuleComponent* Capsule = Character->GetCapsuleComponent(); 
		FVector CapsuleScale = Capsule->GetComponentScale();
	
		//Lower the capsule scale in the Z axis to make the character smaller first
		if(CapsuleScale.Z < 1.f)
		{
			CapsuleScale.Z += DeltaTime * CapsuleScaleChangeRate;
			Capsule->SetWorldScale3D(CapsuleScale);
		}

		else
		{
			CapsuleScale.Z = 1.f;
			Capsule->SetWorldScale3D(CapsuleScale);
			CharacterStance = EStanceStanding; 
		}
	}
}

void UStealth_Movement_Component::AttatchToPlayer(AStealth_Character* CharacterPtr)
{
	Character = CharacterPtr;
}


// Called every frame
void UStealth_Movement_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	
	if(Character != nullptr)
	{
		Move();

		//Stance based processing 
		switch(CharacterStance)
		{
			//Attempting Crouch
		case 1:
			Crouch(DeltaTime);

			break;
			//Attempting Stand
		case 2:
			UnCrouch(DeltaTime); 
			break;

			//Crouched 
		case 3:
			{
				//Can Stoop
				bool Result = false;
				FVector StoopObjectPosition = FVector(0.f,0.f,0.f);

				CanStoop(Result,StoopObjectPosition);

				if(Result)
				{
					UE_LOG(LogTemp,Warning,TEXT("Object Position: %s"),*StoopObjectPosition.ToString());
					StoopTarget = StoopObjectPosition; 
					CharacterStance = EStanceAttemptingStoop;
				} 
				//StoopCheckAndMove(DeltaTime); 
				break;
			}

		//Attempting To Stoop 
		case 4:
			{
				if(StoopedToTarget(DeltaTime))
				{
					//At Target
					CharacterStance = EStanceStooped;
				}
				break;
			}

			//Attempting to stand from stoop
		case 5:
			{
				//Rise
				if(RiseToTarget(DeltaTime))
				{
					if(Character->GetCapsuleComponent()->GetComponentScale().Z >=.6f)
					{
						CharacterStance = EStanceCrouched; 
					}
					else if(Character->GetCapsuleComponent()->GetComponentScale().Z <.6f)
					{
						CharacterStance = EStanceStooped;
					}
				}
				break;
			}

			//Stooped
		case 6:
			{
				//Can Stoop
				bool Result = false;
				FVector StoopObjectPosition = FVector(0.f,0.f,0.f);

				CanStoop(Result,StoopObjectPosition);

				if(Result)
				{	//StoopTarget = StoopObjectPosition; 
					//CharacterStance = EStanceAttemptingStoop;
					
					FVector Object = GetObjectAbovePlayer(Result);
					//If Not under an object Rise
					if(FMath::Abs(StoopTarget.Z- Object.Z )< 1.f)
					{
						CharacterStance = EStanceStoopedUnderObject; 
					}
				}
				else
				{
					RiseTarget = GetTopOfPlayerStanding(); 
					CharacterStance = EStanceAttemptingStandFromStoop;
				}
				break; 
			}
			//Stooped under object
		case 7:
			{
				bool Result = false;
				FVector StoopObjectPosition = FVector(0.f,0.f,0.f);
				CanStoop(Result,StoopObjectPosition);

				if(Result)
				{
					FVector Object = GetObjectAbovePlayer(Result);

					if(abs(Object.Z - StoopObjectPosition.Z) < 1.f && (abs(GetTopOfPlayerScaled().Z - Object.Z ) > 5.f))
					{
						RiseTarget = Object;
						CharacterStance = EStanceAttemptingStoopHigher;
						break; 
					}

					if(!(abs(Object.Z - StoopObjectPosition.Z) < 1.f)&& (abs(GetTopOfPlayerScaled().Z - StoopObjectPosition.Z ) > 5.f))
					{
						StoopTarget = StoopObjectPosition; 
						CharacterStance = EStanceAttemptingStoopLower;
					}
					break; 
				}
				else
				{
					FVector Object = GetObjectAbovePlayer(Result);
					//UE_LOG(LogTemp,Warning,TEXT("Object Height: %f"),Object.Z );

					if(!Result)
					{
						if(Object.Z > GetTopOfPlayerStanding().Z)
						{
							CharacterStance = EStanceStooped;
						}
					}
				}
				
				break; 
			}
			//Attempting stoop lower
		case 8:
			{
				if(StoopedToTarget(DeltaTime))
				{
					//At Target
					CharacterStance = EStanceStoopedUnderObject;
				}
				break;
				
			}
			//Attempting Stoop higher
		case 9:
			{
				if(RiseToTarget(DeltaTime))
				{CharacterStance = EStanceStoopedUnderObject;}
				break; 
			}
		default:
			break;
			
		}


		//UE_LOG(LogTemp,Warning,TEXT("Top Of Player Capsule: %s"),*GetTopOfPlayer().ToString());


		//Ability Based Processing
		switch(CharacterMovementAbility)
		{
			//Neutral
		case 0:
			break;
			
			//Leaning
		case 1:
			LeanCheckAndMove(DeltaTime);
			break;
			
			//Returning From Lean 
		case 2:
			ReturnFromLean(DeltaTime); 
			break;
			
			//Sprinting 
		case 3:
			break;
		
		default: break;
			
		}

		//Vaulting
		bool bCanVault = false;
		FVector TempEndLocation = FVector(0.f,0.f,0.f);

		switch(VaultingState)
		{
			//Not Vaulting
			case 0:

				if(CharacterStance == EStanceStanding || CharacterStance == EStanceCrouched)
				{
					CanVault(bCanVault, TempEndLocation);
					if(bCanVault)
					{
						Ending_Location = TempEndLocation;
						VaultingState = Vaulst_Can_Vault;
						AbilityPopupHUD->AddToPlayerScreen(); 

						//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Can Vault "),false);

					}
					else
					{
						VaultingState = Vaulst_Not_Vaulting; 

					}
				}
			break;


		
			//Can Vault
			case 1:
				CanVault(bCanVault, TempEndLocation);
			if(bCanVault)
			{
				Ending_Location = TempEndLocation;
				VaultingState = Vaulst_Can_Vault; 
				//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Can Vault "),false);

			}
			else
			{
				AbilityPopupHUD->RemoveFromParent(); 
				VaultingState = Vaulst_Not_Vaulting; 

			}
			break;
			//Vaulting 
			case 2:
				//	UE_LOG(LogTemp,Warning,TEXT("Can Vault"));

				TickVault(DeltaTime); 
			break; 
		}

		//Handle Debug
		if(bShowDebug)
		{
			if(MovementDebugHUD)
			{
				UCapsuleComponent* CharacterCapsule = Character->GetCapsuleComponent(); 
				MovementDebugHUD->TickUpdate(Character->GetActorLocation(),CharacterCapsule->GetComponentRotation(),CharacterCapsule->GetComponentScale().Z,CharacterCapsule->GetScaledCapsuleHalfHeight(),CharacterCapsule->GetScaledCapsuleRadius(),Character->GetFirstPersonCameraComponent()->GetRelativeLocation().Z,Character->GetFirstPersonCameraComponent()->GetRelativeRotation().Roll,Character->GetFirstPersonCameraComponent()->GetComponentRotation(),CharacterStance,CharacterMovementAbility,VaultingState);
			
			}
		}
		
	}

	

	// ...
}


void UStealth_Movement_Component::LeanCheckAndMove(float DeltaTime) const
{
	
	UCameraComponent* CharacterCamera = Character->GetFirstPersonCameraComponent(); 
	FVector CameraPosition = CharacterCamera->GetRelativeLocation();
	
	//This is so the lean diagonals give more of a diamond shape than a square
	//It means if you're trying to lean in two directions (up and right e.g) you go half the distance but get that angle
	
	FVector2D RefinedTarget; 
	if((LeanTarget.X!=CameraNeutralLean.Y )&&(LeanTarget.Y!=CameraNeutralLean.Z)) {RefinedTarget = FVector2d(CameraNeutralLean.Y,CameraNeutralLean.Z)+((LeanTarget - FVector2d(CameraNeutralLean.Y,CameraNeutralLean.Z))/2);}
	else{RefinedTarget = LeanTarget;}


	//Get Difference between current and desired location
	const FVector Difference = FVector(CameraNeutralLean.X,RefinedTarget.X,RefinedTarget.Y) -CameraPosition;

	//Inerpolate towards the desired camera position until the distance is within an acceptable margin to just teleport the rest. 
	if(Difference.Length() > .01f)
	{
		CameraPosition += DeltaTime * Difference * LeanRate;
		CharacterCamera->SetRelativeLocation(CameraPosition);
	}
	else
	{
		CharacterCamera->SetRelativeLocation(FVector(CameraNeutralLean.X,RefinedTarget.X,RefinedTarget.Y));	
	}
		
	//Set Camera Roll so that it feels more like an angled lean
	//Instead of interpolating both the angle and location and trying to get them to line up I just interpolate the location
	//and use its progress to get a roll angle for the camera.
	
	const FRotator CurrentRotation = CharacterCamera->GetRelativeRotation();
	if(CameraPosition.Y != CameraNeutralLean.Y)
	{
		//Get Percentage of the total journey from origin to target to know what angle to set the camera to.
		const float XLeanDifference = CameraNeutralLean.Y-CameraPosition.Y;
		const float LeanPercentage = abs(XLeanDifference)/LeanDistance;

		if(XLeanDifference > 0){CharacterCamera->SetRelativeRotation_Direct(FRotator(CurrentRotation.Pitch,CurrentRotation.Yaw,LeanAngle*LeanPercentage*-1));}
		else{CharacterCamera->SetRelativeRotation_Direct(FRotator(CurrentRotation.Pitch,CurrentRotation.Yaw,LeanAngle*LeanPercentage));}
	}
	else{CharacterCamera->SetRelativeRotation(FRotator(CurrentRotation.Pitch,CurrentRotation.Yaw,0.f));}
	
}

void UStealth_Movement_Component::ReturnFromLean(float DeltaTime)
{
	//Sets the target back to the neutral position 
	LeanTarget = FVector2d(CameraNeutralLean.Y,HeadBobbingDefaultHeight);
	LeanCheckAndMove(DeltaTime);
	
	UCameraComponent* CharacterCamera = Character->GetFirstPersonCameraComponent(); 

	//If it is within an acceptable margin we snap it back to its right position and make some tweaks to get head bobbing working again. 
	if(	abs((FVector(CameraNeutralLean.X,LeanTarget.X,LeanTarget.Y) - CharacterCamera->GetRelativeLocation()).Length()) < 1.0f)
	{
		const FRotator CurrentRotation = CharacterCamera->GetRelativeRotation(); 
		CharacterMovementAbility = EmaNeutral;
		CharacterCamera->SetRelativeRotation(FRotator(CurrentRotation.Pitch,CurrentRotation.Yaw,0.f));
		bHeadBobbingUp = true;
	}
}

bool UStealth_Movement_Component::StoopedToTarget(float DeltaTime)
{
	if(Character != nullptr)
	{
		UCapsuleComponent* CharacterCapsule = Character->GetCapsuleComponent();

		//Check if the height is right
		if(GetTopOfPlayerScaled().Z +2.f >= StoopTarget.Z  && CharacterCapsule->GetComponentScale().Z > StoopMinScale){
		//Reduce Scale
				CharacterCapsule->SetWorldScale3D(CharacterCapsule->GetComponentScale() - FVector(0.f,0.f,DeltaTime *StoopRate));
			return false; 
		}
		else
		{
			return true;
		}
	}
	else{return false;} 
}

FVector UStealth_Movement_Component::GetTopOfPlayerScaled()
{
	if(Character != nullptr)
	{
		const UCapsuleComponent*  CharacterCapsule = Character->GetCapsuleComponent(); 
		FVector CapsuleLocation = CharacterCapsule->GetComponentLocation();
		CapsuleLocation.Z =  CapsuleLocation.Z + CharacterCapsule->GetScaledCapsuleHalfHeight() -0.5f;
		return CapsuleLocation; 
	}
	return FVector(0.f,0.f,0.f); 
}

FVector UStealth_Movement_Component::GetTopOfPlayerStanding()
{
	if(Character != nullptr)
	{
		const UCapsuleComponent*  CharacterCapsule = Character->GetCapsuleComponent();  
		FVector CapsuleLocation = CharacterCapsule->GetComponentLocation();
		CapsuleLocation.Z =  CapsuleLocation.Z + CharacterCapsule->GetUnscaledCapsuleHalfHeight() -0.5f;
		return CapsuleLocation; 
	}
	return FVector(0.f,0.f,0.f); 
}

FVector UStealth_Movement_Component::GetBottomOfPlayer()
{
	if(Character != nullptr)
	{
		const UCapsuleComponent*  CharacterCapsule = Character->GetCapsuleComponent(); 
		FVector CapsuleLocation = CharacterCapsule->GetComponentLocation();
		CapsuleLocation.Z =  CapsuleLocation.Z - CharacterCapsule->GetScaledCapsuleHalfHeight()+0.5f ;
		return CapsuleLocation; 
	}
	return FVector(0.f,0.f,0.f); 
}

bool UStealth_Movement_Component::RiseToTarget(float DeltaTime)
{
	if(Character != nullptr)
	{
		UCapsuleComponent* CharacterCapsule = Character->GetCapsuleComponent();

		//Check if the height is right
		if(GetTopOfPlayerScaled().Z +5.f<= RiseTarget.Z){
			//Reduce Scale
			CharacterCapsule->SetWorldScale3D(CharacterCapsule->GetComponentScale() + FVector(0.f,0.f,DeltaTime *StoopRate*.5f));
			return false; 
		}
		else
		{
			return true;
		}
	}
	else{return false;} 
}

FVector UStealth_Movement_Component::GetObjectAbovePlayer(bool &ObjectStraightAbove)
{
	FHitResult Hit1; 
	FVector CharacterLocation = GetTopOfPlayerScaled();
	FVector EndLocation = CharacterLocation + FVector(0.f,0.f,1000.f);
	if(GetWorld()->LineTraceSingleByChannel(Hit1, CharacterLocation, EndLocation, ECC_WorldStatic, RaycastCollisionParams))
	{
		if(FVector::Distance(CharacterLocation,Hit1.Location) <5.f)
		{
			ObjectStraightAbove = true;
			return Hit1.Location;

		}
		ObjectStraightAbove = false;  
		return Hit1.Location;
	}
	ObjectStraightAbove = false;  
	return EndLocation; 
}

void UStealth_Movement_Component::CanStoop(bool& Result, FVector& ObjectPosition)
{
	if(Character != nullptr)
	{
		FVector CharacterLocation = Character->GetCapsuleComponent()->GetComponentLocation();
		FVector ForwardVector = Character->GetFirstPersonCameraComponent()->GetForwardVector() * FVector(70.f,70.f,1.f) ;
		FVector StartingPoint = FVector(CharacterLocation.X,CharacterLocation.Y,GetBottomOfPlayer().Z);
		FVector EndingPoint = StartingPoint+(ForwardVector*.7f);

		FHitResult Hit1;
		FHitResult Hit2;
		
		//Check Infront Player to see if the space is free
		if(!(GetWorld()->LineTraceSingleByChannel(Hit1, StartingPoint, EndingPoint, ECC_WorldStatic, RaycastCollisionParams)))
		{
			if(bShowDebug)
			{
				DrawDebugLine(GetWorld(), StartingPoint, EndingPoint, FColor::Green, false,0.001f, 0, 2);
			}
			//Check Up to see the height of the gap

			FVector UpEndingPoint = FVector(EndingPoint.X,EndingPoint.Y,GetTopOfPlayerStanding().Z);

			if(GetWorld()->LineTraceSingleByChannel(Hit2, EndingPoint, UpEndingPoint, ECC_WorldStatic, RaycastCollisionParams))
			{
				if(bShowDebug)
				{
					DrawDebugLine(GetWorld(), EndingPoint, UpEndingPoint, FColor::Green, false,0.001f, 0, 2);
				}

				//Get Height from feet
				float ObjectHeight = Hit2.Location.Z - EndingPoint.Z;
				if(ObjectHeight > 49.00f)
				{

					Result = true;
					ObjectPosition = Hit2.Location; 
				}
				
			}
			else
			{
				if(bShowDebug)
				{
					DrawDebugLine(GetWorld(), EndingPoint, UpEndingPoint, FColor::Red, false,0.001f, 0, 2);
				}

			}
		}
		else
		{
			if(bShowDebug)
			{
				DrawDebugLine(GetWorld(), StartingPoint, EndingPoint, FColor::Red, false,0.001f, 0, 2);
			}

		}
	}
}

void UStealth_Movement_Component::StartLeaning()
{
	if(Character != nullptr)
	{
		CharacterMovementAbility = EmaLeaning; 
		LastBobbingHeight = Character->GetFirstPersonCameraComponent()->GetRelativeLocation().Z;
	}
}

void UStealth_Movement_Component::StopLeaning()
{
	CharacterMovementAbility = EmaReturningFromLean; 
}

void UStealth_Movement_Component::StartSprinting()
{
	if(CharacterMovementAbility != EmaLeaning)
	{
		CharacterMovementAbility = EmaSprinting; 
	}
}

void UStealth_Movement_Component::StopSprinting()
{
	CharacterMovementAbility = EmaNeutral; 
}

