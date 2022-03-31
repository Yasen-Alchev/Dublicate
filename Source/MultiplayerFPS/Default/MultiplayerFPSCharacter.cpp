#include "MultiplayerFPSCharacter.h"
#include "MultiplayerFPSGameInstance.h"
#include "MultiplayerFPSPlayerController.h"
#include "MultiplayerFPSInGameHUD.h"
#include "MultiplayerFPSHealthSystem.h"
#include "MultiplayerFPSFirearm.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////// PASS CONTROLLER TO FUNCTION CHANGE WEAPON (Character_C_5 that is not on the scene, where as Character_C_2 is the character we want to modify)

AMultiplayerFPSCharacter::AMultiplayerFPSCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

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

	HealthSystem->SetIsReplicated(true);
	HealthSystem->SetNetAddressable();

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	bIsInOptionsMenu = false;
	bIsInBuyMenu = false;
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
}

void AMultiplayerFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiplayerFPSCharacter::InitTeam()
{
	ServerSpawnFirearmActor();
}

void AMultiplayerFPSCharacter::OnHealthChanged(UMultiplayerFPSHealthSystem* HealthSystemComp, float health,
                                               float damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if ((health <= 0) && (!bDead))
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
	PlayerInputComponent->BindAction("OpenBuyMenu", IE_Pressed, this, &AMultiplayerFPSCharacter::ToggleBuyMenu);

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
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ServerSpawnFirearmActor_Implementation !IsValid(FirearmActor)"));
			return;
		}

		AMultiplayerFPSFirearm* Firearm = Cast<AMultiplayerFPSFirearm>(FirearmActor);
		if (!IsValid(Firearm))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ServerSpawnFirearmActor_Implementation !IsValid(Firearm)"));
			return;
		}

		FirearmArray.Add(Firearm);
		if (!IsValid(FirearmArray[i]))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ServerSpawnFirearmActor_Implementation !IsValid(FirearmArray[i])"));
			return;
		}
	}

	FirearmArray[0]->GunMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	FirearmArray[1]->GunMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("BackAttach"));

	UE_LOG(LogTemp, Error, TEXT("%s FirearmArray.NUM() = %d called !!! "), *GetName(), FirearmArray.Num());

	CanFireFirearmArray.Init(true, FirearmArray.Num());
}

void AMultiplayerFPSCharacter::ServerSpawnFirearmActor_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("%s ServerSpawnFirearmActor_Implementation called !!!"), *GetName());
	if (HasAuthority())
	{
		ClientSpawnFirearmActor();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ServerSpawnFirearmActor_Implementation does Not have Authority !!!"));
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

void AMultiplayerFPSCharacter::SetBuyMenuVisibility(bool Visibility)
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(GetController());
		if (IsValid(PlayerController))
		{
			AMultiplayerFPSInGameHUD* InGameHUD = Cast<AMultiplayerFPSInGameHUD>(PlayerController->GetHUD());
			if (IsValid(InGameHUD))
			{
				InGameHUD->SetBuyMenuVisibility(Visibility);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ToggleBuyMenu() -> InGameHUD is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ToggleBuyMenu() -> PlayerController is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ToggleBuyMenu() -> World is not Valid !!!"));
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

void AMultiplayerFPSCharacter::ToggleBuyMenu()
{
	bIsInBuyMenu = !bIsInBuyMenu;
	SetBuyMenuVisibility(bIsInBuyMenu);
	AMultiplayerFPSPlayerController* MyController = Cast<AMultiplayerFPSPlayerController>(GetController());
	if (!IsValid(MyController))
	{
		UE_LOG(LogTemp, Error, TEXT("AAMultiplayerFPSCharacter::ToggleBuyMenu() -> MyController is not Valid !!!"));
	}
	MyController->SetShowMouseCursor(bIsInBuyMenu);
	MyController->ClientIgnoreLookInput(bIsInBuyMenu);
	MyController->ClientIgnoreMoveInput(bIsInBuyMenu);
	bIsInBuyMenu ? MyController->SetInputMode(FInputModeUIOnly()) : MyController->SetInputMode(FInputModeGameOnly());
}

void AMultiplayerFPSCharacter::ServerChangeWeapon_Implementation(int32 slotIndex, TSubclassOf<AMultiplayerFPSFirearm> WeaponClass)
{
	if(HasAuthority())
	{
		ClientChangeWeapon(slotIndex, WeaponClass);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s ServerChangeWeapon_Implementation -> Has No Authority"), *GetName());
	}
}

void AMultiplayerFPSCharacter::ClientChangeWeapon_Implementation(int32 slotIndex, TSubclassOf<AMultiplayerFPSFirearm> WeaponClass)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("ClientChangeWeapon_Implementation Called !!!")));
	if (!IsValid(WeaponClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSCharacter::ClientChangeWeapon -> WeaponClass is not Valid !!!"));
		return;
	}

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ActorSpawnParameters.Owner = this;
	const FVector WeaponLocationVector = FVector(0.0f, 0.0f, 0.0f);
	const FRotator WeaponRotationRotator = FRotator(0.0f, 0.0f, 0.0f);

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSCharacter::ClientChangeWeapon -> World is not Valid !!!"));
		return;
	}
	if (FirearmArray.Num() > slotIndex)
	{
		FirearmArray[slotIndex]->GunMesh->DestroyComponent();
		FirearmArray[slotIndex]->Destroy();
		FirearmArray[slotIndex]->DestroyConstructedComponents();

		AActor* FirearmActor = World->SpawnActor(WeaponClass, &WeaponLocationVector, &WeaponRotationRotator, ActorSpawnParameters);
		if (!IsValid(FirearmActor))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ClientChangeWeapon !IsValid(FirearmActor)"));
			return;
		}

		AMultiplayerFPSFirearm* Firearm = Cast<AMultiplayerFPSFirearm>(FirearmActor);
		if (!IsValid(Firearm))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ClientChangeWeapon !IsValid(Firearm)"));
			return;
		}
		FirearmArray[slotIndex] = Firearm;
		if (slotIndex == 0)
		{
			FirearmArray[slotIndex]->GunMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}
		else if (slotIndex == 1)
		{
			FirearmArray[slotIndex]->GunMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("BackAttach"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSCharacter::ClientChangeWeapon -> FirearmArray[slotIndex] Unknown slotIndex == %d > 2 !!!"), slotIndex);
		}
		CanFireFirearmArray.Init(true, FirearmArray.Num());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSCharacter::ClientChangeWeapon -> FirearmArray is Out Of Bound !!!"), *GetName());
	}
}

void AMultiplayerFPSCharacter::DestoryPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("%s DestoryPlayer -> FirearmArray.Num() = %d!!!"), *GetName(), FirearmArray.Num());
	for(auto FireArmActor : FirearmArray)
	{
		FireArmActor->DestroyConstructedComponents();
		FireArmActor->Destroy();
	}
	DestroyConstructedComponents();
	Destroy(true);
}

void AMultiplayerFPSCharacter::ClientDestoryPlayer_Implementation()
{
	DestoryPlayer();
}

void AMultiplayerFPSCharacter::StartFiring()
{
	UE_LOG(LogTemp, Warning, TEXT("FirearmArray.Num() = %d  >  WeaponInHand = %d"), FirearmArray.Num(), WeaponInHand);
	UE_LOG(LogTemp, Warning, TEXT("CanFireFirearmArray.Num() = %d  >  WeaponInHand = %d"), CanFireFirearmArray.Num(), WeaponInHand);
	if (this->FirearmArray.Num() > this->WeaponInHand && this->CanFireFirearmArray.Num() > this->WeaponInHand)
	{
		if (this->CanFireFirearmArray[this->WeaponInHand])
		{
			this->FirearmArray[(this->WeaponInHand)]->StartFiring();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSCharacter::StartFiring() -> CanFireFirearmArray is Flase !"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSCharacter::StartFiring() -> Arrays Out Of Bound !!!"));
		UE_LOG(LogTemp, Warning, TEXT("FirearmArray.Num() = %d  >  WeaponInHand = %d"), FirearmArray.Num(), WeaponInHand);
		UE_LOG(LogTemp, Warning, TEXT("CanFireFirearmArray.Num() = %d  >  WeaponInHand = %d"), CanFireFirearmArray.Num(), WeaponInHand);
	}
}

void AMultiplayerFPSCharacter::StopFiring()
{
	if(FirearmArray.Num() > WeaponInHand)
	{
		this->FirearmArray[this->WeaponInHand]->StopFiring();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSCharacter::StopFiring() -> Array Out Of Bound !!!"));
		UE_LOG(LogTemp, Warning, TEXT("FirearmArray.Num() = %d  >  WeaponInHand = %d"), FirearmArray.Num(), WeaponInHand);
	}
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
	if(CanFireFirearmArray.Num() > WeaponInHand)
	{
		if (this->CanFireFirearmArray[this->WeaponInHand])
		{
			this->bIsZoomedIn = true;
			this->FirearmArray[(this->WeaponInHand)]->Zoom();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSCharacter::Zoom() -> CanFireFirearmArray Out Of Bound!!! CanFireFirearmArray.Num() = %d > WeaponInHand = %d!"), CanFireFirearmArray.Num(), WeaponInHand);
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