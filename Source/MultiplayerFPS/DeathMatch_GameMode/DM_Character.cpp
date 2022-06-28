#include "DM_Character.h"

#include "DM_InGameHUD.h"
#include "DM_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameInstance.h"
#include "Net/UnrealNetwork.h"


ADM_Character::ADM_Character()
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

void ADM_Character::BeginPlay()
{
	Super::BeginPlay();

	UMultiplayerFPSGameInstance* GameInstanceVar = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (IsValid(GameInstanceVar))
	{
		this->PlayerName = GameInstanceVar->PlayerName;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_Character::BeginPlay() -> GameInstanceVar is not Valid !!!"));
	}
}

void ADM_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ADM_Character::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADM_Character, bIsSprinting);
	DOREPLIFETIME(ADM_Character, bDead);
}


void ADM_Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ADM_Character::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ADM_Character::SprintStop);
	PlayerInputComponent->BindAction("ShowStats", IE_Pressed, this, &ADM_Character::ToggleLeaderBoardVisibility);
	PlayerInputComponent->BindAction("ShowStats", IE_Released, this, &ADM_Character::ToggleLeaderBoardVisibility);
	PlayerInputComponent->BindAction("ShowOptions", IE_Pressed, this, &ADM_Character::ToggleOptionsMenu);


	PlayerInputComponent->BindAxis("MoveForward", this, &ADM_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADM_Character::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADM_Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADM_Character::LookUpAtRate);
}


void ADM_Character::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADM_Character::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADM_Character::MoveForward(float Value)
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

void ADM_Character::MoveRight(float Value)
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

void ADM_Character::SprintStart()
{
	bIsSprinting = true;
}

void ADM_Character::SprintStop()
{
	bIsSprinting = false;
}

void ADM_Character::SetOptionsMenuVisibility(bool Visibility)
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		ADM_PlayerController* PlayerController = Cast<ADM_PlayerController>(this->GetController());
		if (IsValid(PlayerController))
		{
			ADM_InGameHUD* InGameHUD = Cast<ADM_InGameHUD>(PlayerController->GetHUD());
			if (IsValid(InGameHUD))
			{
				InGameHUD->SetOptionMenuVisibility(Visibility);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ADM_Character::SetOptionsMenuVisibility(bool Visibility) -> InGameHUD is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_Character::SetOptionsMenuVisibility(bool Visibility) -> PlayerController is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_Character::SetOptionsMenuVisibility(bool Visibility) -> World is not Valid !!!"));
	}
}

void ADM_Character::ToggleLeaderBoardVisibility()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		ADM_PlayerController* PlayerController = Cast<ADM_PlayerController>(this->GetController());
		if (IsValid(PlayerController))
		{
			ADM_InGameHUD* InGameHUD = Cast<ADM_InGameHUD>(PlayerController->GetHUD());
			if (IsValid(InGameHUD))
			{
				InGameHUD->ToggleLeaderBoardVisibility();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ADM_Character::ToggleLeaderBoardVisibility() -> InGameHUD is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_Character::ToggleLeaderBoardVisibility() -> PlayerController is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_Character::ToggleLeaderBoardVisibility() -> World is not Valid !!!"));
	}
}

void ADM_Character::ToggleOptionsMenu()
{
	bIsInOptionsMenu = !bIsInOptionsMenu;
	SetOptionsMenuVisibility(bIsInOptionsMenu);
	ADM_PlayerController* MyController = Cast<ADM_PlayerController>(GetController());
	if (IsValid(MyController))
	{
		MyController->SetShowMouseCursor(bIsInOptionsMenu);
		MyController->ClientIgnoreLookInput(bIsInOptionsMenu);
		MyController->ClientIgnoreMoveInput(bIsInOptionsMenu);
		bIsInOptionsMenu ? MyController->SetInputMode(FInputModeUIOnly()) : MyController->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_Character::ToggleOptionsMenu() -> MyController is not Valid !!!"));
	}
}