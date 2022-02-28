// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPS_MultiplayCharacter.h"

#include "FPS_MultiplayGameInstance.h"
#include "FPS_MultiplayPlayerController.h"
#include "FPS_MultiplayInGameHUD.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// AFPS_MultiplayCharacter

AFPS_MultiplayCharacter::AFPS_MultiplayCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	bIsInOptionsMenu = false;
	bIsSprinting = false;
	bDead = false;
	Team = TEAM_NONE;
}

void AFPS_MultiplayCharacter::BeginPlay()
{
	Super::BeginPlay();

	UFPS_MultiplayGameInstance* GameInstanceVar = Cast<UFPS_MultiplayGameInstance>(GetGameInstance());
	if (IsValid(GameInstanceVar))
	{
		this->PlayerName = GameInstanceVar->PlayerName;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayCharacter::BeginPlay() -> GameInstanceVar is not Valid !!!"));
	}
	UE_LOG(LogTemp, Error, TEXT("%s BeginPlay()"), *PlayerName);
}

void AFPS_MultiplayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPS_MultiplayCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPS_MultiplayCharacter, bIsSprinting);
	DOREPLIFETIME(AFPS_MultiplayCharacter, Team);
	DOREPLIFETIME(AFPS_MultiplayCharacter, bDead);
	DOREPLIFETIME(AFPS_MultiplayCharacter, PlayerName);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFPS_MultiplayCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPS_MultiplayCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPS_MultiplayCharacter::SprintStop);
	PlayerInputComponent->BindAction("ShowLeaderBoard", IE_Pressed, this, &AFPS_MultiplayCharacter::ToggleLeaderBoardVisibility);
	PlayerInputComponent->BindAction("ShowLeaderBoard", IE_Released, this, &AFPS_MultiplayCharacter::ToggleLeaderBoardVisibility);
	PlayerInputComponent->BindAction("ShowOptions", IE_Pressed, this, &AFPS_MultiplayCharacter::ToggleOptionMenu);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPS_MultiplayCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPS_MultiplayCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPS_MultiplayCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPS_MultiplayCharacter::LookUpAtRate);
}

void AFPS_MultiplayCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPS_MultiplayCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFPS_MultiplayCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		if (!bIsSprinting)
		{
			Value *= 0.6f;
		}

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFPS_MultiplayCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		if (!bIsSprinting)
		{
			Value *= 0.6f;
		}

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFPS_MultiplayCharacter::SprintStart()
{
	bIsSprinting = true;
}

void AFPS_MultiplayCharacter::SprintStop()
{
	bIsSprinting = false;
}

void AFPS_MultiplayCharacter::SetOptionsMenuVisibility(bool Visibility)
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		AFPS_MultiplayPlayerController* PlayerController = Cast<AFPS_MultiplayPlayerController>(this->GetController());
		if (IsValid(PlayerController))
		{
			AFPS_MultiplayInGameHUD* InGameHUD = Cast<AFPS_MultiplayInGameHUD>(PlayerController->GetHUD());
			if (IsValid(InGameHUD))
			{
				InGameHUD->SetOptionMenuVisibility(Visibility);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayCharacter::SetOptionsMenuVisibility(bool Visibility) -> InGameHUD is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayCharacter::SetOptionsMenuVisibility(bool Visibility) -> PlayerController is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayCharacter::SetOptionsMenuVisibility(bool Visibility) -> World is not Valid !!!"));
	}
}

void AFPS_MultiplayCharacter::ToggleLeaderBoardVisibility()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		AFPS_MultiplayPlayerController* PlayerController = Cast<AFPS_MultiplayPlayerController>(this->GetController());
		if (IsValid(PlayerController))
		{
			AFPS_MultiplayInGameHUD* InGameHUD = Cast<AFPS_MultiplayInGameHUD>(PlayerController->GetHUD());
			if (IsValid(InGameHUD))
			{
				InGameHUD->ToggleLeaderBoardVisibility();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayCharacter::ToggleLeaderBoardVisibility() -> InGameHUD is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayCharacter::ToggleLeaderBoardVisibility() -> PlayerController is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayCharacter::ToggleLeaderBoardVisibility() -> World is not Valid !!!"));
	}
}

void AFPS_MultiplayCharacter::ToggleOptionMenu()
{
	bIsInOptionsMenu = !bIsInOptionsMenu;
	SetOptionsMenuVisibility(bIsInOptionsMenu);
	AFPS_MultiplayPlayerController* MyController = Cast<AFPS_MultiplayPlayerController>(GetController());
	if (IsValid(MyController))
	{
		MyController->SetShowMouseCursor(bIsInOptionsMenu);
		MyController->ClientIgnoreLookInput(bIsInOptionsMenu);
		MyController->ClientIgnoreMoveInput(bIsInOptionsMenu);
		bIsInOptionsMenu ? MyController->SetInputMode(FInputModeUIOnly()) : MyController->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayCharacter::ToggleOptionsMenu() -> MyController is not Valid !!!"));
	}
}

void AFPS_MultiplayCharacter::InitTeam()
{
	AFPS_MultiplayPlayerController* PlayerController = Cast<AFPS_MultiplayPlayerController>(this->GetController());
	if (IsValid(PlayerController))
	{
		this->Team = PlayerController->Team;
		FString TheTEAMNAME = (Team == TEAM_BLUE ? "BLUE" : Team == TEAM_RED ? "RED" : "NONE");
		UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayCharacter::InitTeam(AActor * Player) -> TEAM = %s !!!"), *PlayerName, *TheTEAMNAME);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayCharacter::InitTeam(AActor * Player)->PlayerController is not Valid !!!"), *PlayerName);
	}

	UE_LOG(LogTemp, Error, TEXT("Player Name: %s"), *PlayerName);
}

