#include "MultiplayerFPSCharacter.h"
#include "MultiplayerFPSGameInstance.h"
#include "MultiplayerFPSPlayerController.h"
#include "MultiplayerFPSInGameHUD.h"
#include "MultiplayerFPSHealthSystem.h"
#include "MultiplayerFPSFirearm.h"
#include "MultiplayerFPSGameMode.h"
#include "MultiplayerFPSGameState.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "../CommonClasses/HealthPickup.h"


AMultiplayerFPSCharacter::AMultiplayerFPSCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	this->FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	this->FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	this->FirstPersonCamera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	this->FirstPersonCamera->bUsePawnControlRotation = true;

	this->FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	this->FirstPersonMesh->SetOnlyOwnerSee(true);
	this->FirstPersonMesh->SetupAttachment(this->FirstPersonCamera);
	this->FirstPersonMesh->bCastDynamicShadow = false;
	this->FirstPersonMesh->CastShadow = false;
	this->FirstPersonMesh->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	this->FirstPersonMesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	this->FullBodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FullBodyMesh"));
	this->FullBodyMesh->SetOwnerNoSee(true);
	this->FullBodyMesh->SetupAttachment(this->FirstPersonCamera);
	this->FullBodyMesh->bCastDynamicShadow = true;
	this->FullBodyMesh->CastShadow = true;

	this->HealthSystem = CreateDefaultSubobject<UMultiplayerFPSHealthSystem>(TEXT("HealthSystem"));
	HealthSystem->OnHealthChangedEvent.AddDynamic(this, &AMultiplayerFPSCharacter::OnHealthChanged);

	this->OnHealEvent.AddDynamic(this, &AMultiplayerFPSCharacter::Heal);

	this->BodyHitboxBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyHitboxBox"));
	this->BodyHitboxBox->SetBoxExtent(FVector(28.0f, 40.0f, 73.0f));
	this->BodyHitboxBox->SetRelativeLocation(FVector(0.0f, 0.0f, -18.0f));
	this->BodyHitboxBox->SetupAttachment(RootComponent);
	this->BodyHitboxBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	HealthSystem->SetIsReplicated(true);
	HealthSystem->SetNetAddressable();

	this->HeadHitboxBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadHitboxBox"));	
	this->HeadHitboxBox->SetBoxExtent(FVector(16.0f, 16.0f, 16.0f));
	this->HeadHitboxBox->SetRelativeLocation(FVector(5.0f, 4.0f, 70.0f));
	this->HeadHitboxBox->SetupAttachment(RootComponent);	
	this->HeadHitboxBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	bIsInOptionsMenu = false;
	bIsSprinting = false;
	bDead = false;

	bNetUseOwnerRelevancy = true;

	this->WeaponInHand = 0;

	this->bIsReloading = false;
	this->bIsZoomedIn = false;
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
	
	ServerSpawnFirearmActor();
}

void AMultiplayerFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiplayerFPSCharacter::OnHealthChanged(UMultiplayerFPSHealthSystem* HealthSystemComp, float Health,
	float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if ((Health <= 0) && (!bDead))
	{
		UWorld* World = GetWorld();
		if (!IsValid(World))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::OnHealthChanged() -> World is not Valid !!!"));
			return;
		}
		if(!IsValid(GEngine))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::OnHealthChanged() -> GEngine is not Valid !!!"));
			return;
		}

		AMultiplayerFPSPlayerController* const PlayerController = Cast<AMultiplayerFPSPlayerController>(GEngine->GetFirstLocalPlayerController(World));
		if (!IsValid(PlayerController))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::OnHealthChanged() -> PlayerControlle2 is not Valid !!!"));
				return;
		}

		AMultiplayerFPSCharacter* AutonomousProxyPlayer = Cast<AMultiplayerFPSCharacter>(PlayerController->GetPawn());
		if (!IsValid(AutonomousProxyPlayer))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::OnHealthChanged() -> AutonomousProxyPlayer is not Valid !!!"));
			return;
		}
		AutonomousProxyPlayer->ServerOnRemoteProxyPlayerDeath(this);
	}
}

float AMultiplayerFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& MovieSceneBlends, 
	AController* EventInstigator, AActor* DamageCauser)
{
	if(!HasAuthority())
	{
		this->HealthSystem->TakeDamage(this, DamageAmount,  nullptr, EventInstigator, DamageCauser);
	}
	return Super::TakeDamage(DamageAmount, MovieSceneBlends, EventInstigator, DamageCauser);
}

void AMultiplayerFPSCharacter::ServerOnPlayerDeath_Implementation()
{
	AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(GetController());
	if(IsValid(PlayerController))
	{
		PlayerController->RespawnPlayer();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ServerOnPlayerDeath_Implementation() -> PlayerController is not Valid !!!"));
	}

}

void AMultiplayerFPSCharacter::Heal(float Value, AActor* HealthPickupActor)
{
	this->HealthSystem->Heal(Value);

	if (!IsValid(HealthPickupActor))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ServerOnPlayerDeath_Implementation !IsValid(HealthPickupActor)"));
		return;
	}

	AHealthPickup* HealthPickup = Cast<AHealthPickup>(HealthPickupActor);
	if (!IsValid(HealthPickup))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ServerOnPlayerDeath_Implementation !IsValid(HealthPickup)"));
		return;
	}

	if (HasAuthority())
	{
		HealthPickup->ClientDestroyHealthPickup();
	}
	HealthPickup->DestroyHealthPickup();
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
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMultiplayerFPSCharacter::StartFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMultiplayerFPSCharacter::StopFiring);
	PlayerInputComponent->BindAction("SwitchFirearm", IE_Pressed, this, &AMultiplayerFPSCharacter::SwitchWeapon);
	PlayerInputComponent->BindAction("SwitchFireMode", IE_Pressed, this, &AMultiplayerFPSCharacter::SwitchFireMode);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMultiplayerFPSCharacter::Reload);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AMultiplayerFPSCharacter::Zoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AMultiplayerFPSCharacter::ZoomOut);
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

void AMultiplayerFPSCharacter::ClientSpawnFirearmActor_Implementation()
{
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ActorSpawnParameters.Owner = this;
	const FVector WeaponLocationVector = FVector(0.0f, 0.0f, 0.0f);
	const FRotator WeaponRotationRotator = FRotator(0.0f, 0.0f, 0.0f);

	for (int32 i = 0; i < FirearmClassArray.Num(); ++i)
	{
		AActor* FirearmActor = GetWorld()->SpawnActor(FirearmClassArray[i], &WeaponLocationVector, &WeaponRotationRotator, ActorSpawnParameters);
		if (!IsValid(FirearmActor))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::BeginPlay !IsValid(FirearmActor)"));
			return;
		}

		AMultiplayerFPSFirearm* Firearm = Cast<AMultiplayerFPSFirearm>(FirearmActor);
		if (!IsValid(Firearm))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::BeginPlay !IsValid(Firearm)"));
			return;
		}

		FirearmArray.Add(Firearm);
		if (!IsValid(FirearmArray[i]))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::BeginPlay !IsValid(FirearmArray[i])"));
			return;
		}
	}

	FirearmArray[0]->GunMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	FirearmArray[1]->GunMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("BackAttach"));

	CanFireFirearmArray.Init(true, FirearmArray.Num());
}

void AMultiplayerFPSCharacter::ServerSpawnFirearmActor_Implementation()
{
	if (HasAuthority())
	{
		ClientSpawnFirearmActor();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ServerSpawnFirearmActor_Implementation() -> HasAuthority() is not Valid !!!"));
	}
}

void AMultiplayerFPSCharacter::ServerOnRemoteProxyPlayerDeath_Implementation(AMultiplayerFPSCharacter* ProxyCharacter)
{
	ProxyCharacter->ServerOnPlayerDeath();
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

void AMultiplayerFPSCharacter::DestoryPlayer()
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors, true);

	for(auto Attached: AttachedActors)
	{
		Attached->Destroy(true);
	}
	Destroy(true);
}

void AMultiplayerFPSCharacter::ClientDestoryPlayer_Implementation()
{
	DestoryPlayer();
}

void AMultiplayerFPSCharacter::StartFiring()
{
	if (this->CanFireFirearmArray[this->WeaponInHand])
	{
		this->FirearmArray[(this->WeaponInHand)]->StartFiring();
	}
}

void AMultiplayerFPSCharacter::StopFiring()
{
	this->FirearmArray[this->WeaponInHand]->StopFiring();
}

void AMultiplayerFPSCharacter::SwitchWeapon()
{
	if (this->WeaponInHand == 0)
	{
		this->WeaponInHand = 1;

		FirearmArray[0]->GunMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("BackAttach"));
		FirearmArray[1]->GunMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	else
	{
		this->WeaponInHand = 0;

		FirearmArray[0]->GunMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		FirearmArray[1]->GunMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("BackAttach"));
	}
}

void AMultiplayerFPSCharacter::SwitchFireMode()
{
	if (!this->bIsReloading)
	{
		this->FirearmArray[this->WeaponInHand]->SwitchFireMode();
	}
}

void AMultiplayerFPSCharacter::Reload()
{
	if (!bIsReloading && FirearmArray[this->WeaponInHand]->ShouldReloadFirearm())
	{
		this->FirearmArray[this->WeaponInHand]->Reload();
		if (bIsZoomedIn)
		{
			UE_LOG(LogTemp, Warning, TEXT("Reloading -> Zoomed Out!"));
			AMultiplayerFPSCharacter::ZoomOut();
		}
	}
}

void AMultiplayerFPSCharacter::Zoom()
{
	if (this->CanFireFirearmArray[this->WeaponInHand])
	{
		this->bIsZoomedIn = true;
		this->FirearmArray[(this->WeaponInHand)]->Zoom();
	}
}

void AMultiplayerFPSCharacter::ZoomOut()
{
	if (this->bIsZoomedIn)
	{
		this->FirearmArray[(this->WeaponInHand)]->ZoomOut();
		this->bIsZoomedIn = false;
	}
}

void AMultiplayerFPSCharacter::SetFOV(float FOV)
{
	FirstPersonCamera->SetFieldOfView(FOV);
}

void AMultiplayerFPSCharacter::HideFPMeshes()
{
	this->FirstPersonMesh->SetVisibility(false, false);
	for (int32 i = 0; i < this->FirearmArray.Num(); ++i)
	{
		this->FirearmArray[i]->GunMesh->SetVisibility(false, false);
	}
}

void AMultiplayerFPSCharacter::ShowFPMeshes()
{
	this->FirstPersonMesh->SetVisibility(true, false);
	for (int32 i = 0; i < this->FirearmArray.Num(); ++i)
	{
		this->FirearmArray[i]->GunMesh->SetVisibility(true, false);
	}
}

void AMultiplayerFPSCharacter::SetIsReloading()
{
	this->bIsReloading = !this->bIsReloading;
	if (this->bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("Started Reloading!"));
		this->CanFireFirearmArray[this->WeaponInHand] = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Firearm Reloaded!"));
		this->CanFireFirearmArray[this->WeaponInHand] = true;
	}
}
