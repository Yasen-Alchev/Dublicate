#include "MultiplayerFPSCharacter.h"
#include "MultiplayerFPSGameInstance.h"
#include "MultiplayerFPSPlayerController.h"
#include "MultiplayerFPSInGameHUD.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"


AMultiplayerFPSCharacter::AMultiplayerFPSCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	bIsInOptionsMenu = false;
	bIsSprinting = false;
	bDead = false;
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
}

void AMultiplayerFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiplayerFPSCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerFPSCharacter, bIsSprinting);
	DOREPLIFETIME(AMultiplayerFPSCharacter, bDead);
}

void AMultiplayerFPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMultiplayerFPSCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMultiplayerFPSCharacter::SprintStop);
	PlayerInputComponent->BindAction("ShowStats", IE_Pressed, this, &AMultiplayerFPSCharacter::ToggleLeaderBoardVisibility);
	//PlayerInputComponent->BindAction("ShowStats", IE_Released, this, &AMultiplayerFPSCharacter::ToggleLeaderBoardVisibility);
	PlayerInputComponent->BindAction("ShowOptions", IE_Pressed, this, &AMultiplayerFPSCharacter::ToggleOptionsMenu);


	PlayerInputComponent->BindAxis("MoveForward", this, &AMultiplayerFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMultiplayerFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMultiplayerFPSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMultiplayerFPSCharacter::LookUpAtRate);
}

void AMultiplayerFPSCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMultiplayerFPSCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMultiplayerFPSCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		if (!bIsSprinting)
		{
			Value *= 0.6f;
		}

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMultiplayerFPSCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		if (!bIsSprinting)
		{
			Value *= 0.6f;
		}

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
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

void AMultiplayerFPSCharacter::ToggleOptionsMenu()
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

