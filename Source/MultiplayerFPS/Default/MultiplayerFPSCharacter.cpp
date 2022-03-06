// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerFPSCharacter.h"

#include "MultiplayerFPSPlayerController.h"
#include "MultiplayerFPSInGameHUD.h"
#include "MultiplayerFPSGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// AMultiplayerFPSCharacter
AMultiplayerFPSCharacter::AMultiplayerFPSCharacter()
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

void AMultiplayerFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UMultiplayerFPSGameInstance* GameInstanceVar = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (IsValid(GameInstanceVar))
	{
		this->PlayerName = GameInstanceVar->PlayerName;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::BeginPlay() -> GameInstanceVar is not Valid !!!"));
	}
	UE_LOG(LogTemp, Error, TEXT("%s BeginPlay()"), *PlayerName);
}

void AMultiplayerFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiplayerFPSCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerFPSCharacter, bIsSprinting);
	DOREPLIFETIME(AMultiplayerFPSCharacter, Team);
	DOREPLIFETIME(AMultiplayerFPSCharacter, bDead);
	DOREPLIFETIME(AMultiplayerFPSCharacter, PlayerName);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMultiplayerFPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMultiplayerFPSCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMultiplayerFPSCharacter::SprintStop);
	PlayerInputComponent->BindAction("ShowLeaderBoard", IE_Pressed, this, &AMultiplayerFPSCharacter::ToggleLeaderBoardVisibility);
	PlayerInputComponent->BindAction("ShowLeaderBoard", IE_Released, this, &AMultiplayerFPSCharacter::ToggleLeaderBoardVisibility);
	PlayerInputComponent->BindAction("ShowOptions", IE_Pressed, this, &AMultiplayerFPSCharacter::ToggleOptionMenu);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMultiplayerFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMultiplayerFPSCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMultiplayerFPSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMultiplayerFPSCharacter::LookUpAtRate);
}

void AMultiplayerFPSCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMultiplayerFPSCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMultiplayerFPSCharacter::MoveForward(float Value)
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

void AMultiplayerFPSCharacter::MoveRight(float Value)
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

void AMultiplayerFPSCharacter::SprintStart()
{
	bIsSprinting = true;
}

void AMultiplayerFPSCharacter::SprintStop()
{
	bIsSprinting = false;
}

void AMultiplayerFPSCharacter::SetOptionsMenuVisibility(bool Visibility)
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(this->GetController());
		if (IsValid(PlayerController))
		{
			AMultiplayerFPSInGameHUD* InGameHUD = Cast<AMultiplayerFPSInGameHUD>(PlayerController->GetHUD());
			if (IsValid(InGameHUD))
			{
				InGameHUD->SetOptionMenuVisibility(Visibility);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::SetOptionsMenuVisibility(bool Visibility) -> InGameHUD is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::SetOptionsMenuVisibility(bool Visibility) -> PlayerController is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::SetOptionsMenuVisibility(bool Visibility) -> World is not Valid !!!"));
	}
}

void AMultiplayerFPSCharacter::ToggleLeaderBoardVisibility()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(this->GetController());
		if (IsValid(PlayerController))
		{
			AMultiplayerFPSInGameHUD* InGameHUD = Cast<AMultiplayerFPSInGameHUD>(PlayerController->GetHUD());
			if (IsValid(InGameHUD))
			{
				InGameHUD->ToggleLeaderBoardVisibility();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ToggleLeaderBoardVisibility() -> InGameHUD is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ToggleLeaderBoardVisibility() -> PlayerController is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ToggleLeaderBoardVisibility() -> World is not Valid !!!"));
	}
}

void AMultiplayerFPSCharacter::ToggleOptionMenu()
{
	bIsInOptionsMenu = !bIsInOptionsMenu;
	SetOptionsMenuVisibility(bIsInOptionsMenu);
	AMultiplayerFPSPlayerController* MyController = Cast<AMultiplayerFPSPlayerController>(GetController());
	if (IsValid(MyController))
	{
		MyController->SetShowMouseCursor(bIsInOptionsMenu);
		MyController->ClientIgnoreLookInput(bIsInOptionsMenu);
		MyController->ClientIgnoreMoveInput(bIsInOptionsMenu);
		bIsInOptionsMenu ? MyController->SetInputMode(FInputModeUIOnly()) : MyController->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ToggleOptionsMenu() -> MyController is not Valid !!!"));
	}
}

void AMultiplayerFPSCharacter::InitTeam()
{
	AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(this->GetController());
	if (IsValid(PlayerController))
	{
		this->Team = PlayerController->Team;
		FString TheTEAMNAME = (Team == TEAM_BLUE ? "BLUE" : Team == TEAM_RED ? "RED" : "NONE");
		UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSCharacter::InitTeam(AActor * Player) -> TEAM = %s !!!"), *PlayerName, *TheTEAMNAME);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSCharacter::InitTeam(AActor * Player)->PlayerController is not Valid !!!"), *PlayerName);
	}

	UE_LOG(LogTemp, Error, TEXT("Player Name: %s"), *PlayerName);
}

