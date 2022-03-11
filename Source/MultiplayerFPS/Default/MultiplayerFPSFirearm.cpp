#include "MultiplayerFPSFirearm.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "../Default/MultiplayerFPSCharacter.h"
#include "../Default/MultiplayerFPSTeamBasedCharacter.h"
#include "../Default/MultiplayerFPSPlayerController.h"

AMultiplayerFPSFirearm::AMultiplayerFPSFirearm()
{
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetOnlyOwnerSee(true);
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

	RootComponent = GunMesh;

	GunMuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	GunMuzzleLocation->SetupAttachment(GunMesh);
	GunMuzzleLocation->SetRelativeLocation(FVector(0.2f, 55.5f, 11.0f));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetBoxExtent(FVector(8.0f, 41.0f, 18.5f));
	BoxCollision->SetRelativeLocation(FVector(0.0f, 14.0f, 4.0f));
	BoxCollision->SetupAttachment(GunMesh);

	this->bShowDebugTrace = false;

	this->BurstCount = 3;
	this->BurstsFired = 0;

	this->BurstFiringInterval = 0.15f;

	this->ReloadTime = 1.0f;
}

void AMultiplayerFPSFirearm::BeginPlay()
{
	Super::BeginPlay();

	this->CurrentFireMode = FireModesArray[0];
	this->CurrentMagazineCapacity = this->MaxMagazineCapacity;
}

void AMultiplayerFPSFirearm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMultiplayerFPSFirearm::StartFiring()
{
	this->bIsFiring = true;

	int32 CurrentFireModeIndex = FireModesArray.Find(CurrentFireMode);
	if (CurrentFireModeIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::StartFiring CurrentFireModeIndex == INDEX_NONE"));
		return;
	}

	if (CurrentFireMode == EFireMode::Auto)
	{
		if (GetWorldTimerManager().IsTimerActive(this->HeldFiringIntervalTimer))
		{
			GetWorldTimerManager().ClearTimer(this->HeldFiringIntervalTimer);
			GetWorldTimerManager().SetTimer(this->HeldFiringIntervalTimer, FTimerDelegate::CreateLambda([&] {
				this->Fire();
				}), this->HeldFiringIntervalsArray[CurrentFireModeIndex], true, 0.0f);
		}
		else
		{
			GetWorldTimerManager().SetTimer(this->HeldFiringIntervalTimer, FTimerDelegate::CreateLambda([&] {
				this->Fire();
				}), this->HeldFiringIntervalsArray[CurrentFireModeIndex], true, 0.0f);
		}
	}
	else if (CurrentFireMode == EFireMode::Burst)
	{
		if (this->bIsHeldArray[CurrentFireModeIndex])
		{
			if (GetWorldTimerManager().IsTimerActive(this->HeldFiringIntervalTimer))
			{
				GetWorldTimerManager().ClearTimer(this->HeldFiringIntervalTimer);
				GetWorldTimerManager().SetTimer(this->HeldFiringIntervalTimer, FTimerDelegate::CreateLambda([&] {
					this->BurstFire();
					}), this->HeldFiringIntervalsArray[CurrentFireModeIndex], true, 0.0f);
			}
			else
			{
				GetWorldTimerManager().SetTimer(this->HeldFiringIntervalTimer, FTimerDelegate::CreateLambda([&] {
					this->BurstFire();
					}), this->HeldFiringIntervalsArray[CurrentFireModeIndex], true, 0.0f);
			}
		}
		else
		{
			this->BurstFire();
		}
	}
	else if (CurrentFireMode == EFireMode::Semi)
	{
		if (this->bIsHeldArray[CurrentFireModeIndex])
		{
			if (GetWorldTimerManager().IsTimerActive(this->HeldFiringIntervalTimer))
			{
				GetWorldTimerManager().ClearTimer(this->HeldFiringIntervalTimer);
				GetWorldTimerManager().SetTimer(this->HeldFiringIntervalTimer, FTimerDelegate::CreateLambda([&] {
					this->Fire();
					}), this->HeldFiringIntervalsArray[CurrentFireModeIndex], true, 0.0f);
			}
			else
			{
				GetWorldTimerManager().SetTimer(this->HeldFiringIntervalTimer, FTimerDelegate::CreateLambda([&] {
					this->Fire();
					}), this->HeldFiringIntervalsArray[CurrentFireModeIndex], true, 0.0f);
			}
		}
		else
		{
			this->Fire();
		}
	}
}

void AMultiplayerFPSFirearm::BurstFire()
{
	if (GetWorldTimerManager().IsTimerActive(this->BurstFiringIntervalTimer))
	{
		return;
	}
	else
	{
		GetWorldTimerManager().SetTimer(this->BurstFiringIntervalTimer, FTimerDelegate::CreateLambda([&] {
			this->Fire();
			}), this->BurstFiringInterval, true, 0.0f);
	}
}

void AMultiplayerFPSFirearm::Fire()
{
	AActor* PlayerActor = GetOwner();
	if (!IsValid(PlayerActor))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::Fire !IsValid(PlayerActor)"));
		return;
	}

	AMultiplayerFPSCharacter* MultiplayerFPSPlayer = Cast<AMultiplayerFPSCharacter>(PlayerActor);
	if (!IsValid(MultiplayerFPSPlayer))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::Fire !IsValid(MultiplayerFPSPlayer)"));
		return;
	}

	
	if (!IsValid(this->FireAnimation))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::Fire !IsValid(this->FireAnimation)"));	
		return;
	}

	UAnimInstance* AnimInstance = MultiplayerFPSPlayer->FirstPersonMesh->GetAnimInstance();
	if (!IsValid(AnimInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::Fire !IsValid(AnimInstance)"));
		return;
	}

	AnimInstance->Montage_Play(FireAnimation, 1.f);

	FVector StartLocation = MultiplayerFPSPlayer->GetFirstPersonCameraComponent()->GetComponentLocation();

	FRotator EndRotation = MultiplayerFPSPlayer->GetFirstPersonCameraComponent()->GetComponentRotation();
	FVector EndLocation = StartLocation + (EndRotation.Vector() * 20000.0f);

	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Bullet Trace")), true, this);
	TraceParams.AddIgnoredActor(this);

	FHitResult HitResult(ForceInit);

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::Fire !IsValid(World)"));
		return;
	}

	bool bHitResult = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
	if (this->bShowDebugTrace)
	{
		DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, 20.0f, ECC_WorldStatic, 0.35f);
	}

	if (CurrentFireMode == EFireMode::Burst)
	{
		this->BurstsFired++;

		if (this->BurstsFired == this->BurstCount)
		{
			GetWorldTimerManager().ClearTimer(this->BurstFiringIntervalTimer);
			this->BurstsFired = 0;
		}
	}

	if (!bHitResult)
	{
		UE_LOG(LogTemp, Error, TEXT("Not A Blocking Hit!"));
		return;
	}
	if (!IsValid(HitResult.GetActor()))
	{
		AActor* HitActor = HitResult.GetActor();
		if (!IsValid(HitActor))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::Fire !IsValid(HitActor)"));
		}

		AMultiplayerFPSTeamBasedCharacter* HitTeamBasedPlayer = Cast<AMultiplayerFPSTeamBasedCharacter>(HitActor);
		if (!IsValid(HitTeamBasedPlayer))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::Fire !IsValid(HitTeamBasedPlayer)"));
		}

		AMultiplayerFPSTeamBasedCharacter* MultiplayerFPSTeamBasedPlayer = Cast<AMultiplayerFPSTeamBasedCharacter>(PlayerActor);
		if (!IsValid(MultiplayerFPSTeamBasedPlayer))
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::Fire !IsValid(MultiplayerFPSTeamBasedPlayer)"));
		}

		if (MultiplayerFPSTeamBasedPlayer->Team != HitTeamBasedPlayer->Team)
		{
			FDamageEvent DamageEvent;

			AMultiplayerFPSPlayerController* MultiplayerFPSPlayerController = Cast<AMultiplayerFPSPlayerController>(MultiplayerFPSTeamBasedPlayer->GetController());

			HitTeamBasedPlayer->TakeDamage(this->Damage, DamageEvent, MultiplayerFPSPlayerController, MultiplayerFPSTeamBasedPlayer);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Team Player"));
		}
	}

	this->CurrentMagazineCapacity = FMath::Clamp(this->CurrentMagazineCapacity - 1, 0, this->MaxMagazineCapacity);

	UE_LOG(LogTemp, Warning, TEXT("Current Magazine Capacity: %d"), this->CurrentMagazineCapacity);

	if (this->CurrentMagazineCapacity == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Insufficient Ammo!"));
		GetWorldTimerManager().ClearTimer(this->BurstFiringIntervalTimer);
		return;
	}
}

void AMultiplayerFPSFirearm::StopFiring()
{
	this->bIsFiring = false;

	if (GetWorldTimerManager().IsTimerActive(this->HeldFiringIntervalTimer))
	{
		GetWorldTimerManager().ClearTimer(this->HeldFiringIntervalTimer);
	}
}

void AMultiplayerFPSFirearm::SwitchFireMode()
{
	int32 FireModesArraySize = FireModesArray.Num();

	if (FireModesArraySize == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Other Firing Mode Available!"));
	}
	else
	{
		int32 CurrentFireModeIndex = FireModesArray.Find(CurrentFireMode);
		if (CurrentFireModeIndex == INDEX_NONE)
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::SwitchFireMode CurrentFireModeIndex == INDEX_NONE"));
			return;
		}

		if ((CurrentFireModeIndex + 1) > (FireModesArraySize - 1))
		{
			CurrentFireMode = FireModesArray[0];
		}
		else
		{
			CurrentFireMode = FireModesArray[CurrentFireModeIndex + 1];
		}

		UE_LOG(LogTemp, Warning, TEXT("Switched Fire Mode"));
	}
}

void AMultiplayerFPSFirearm::Reload()
{
	if (GetWorldTimerManager().IsTimerActive(BurstFiringIntervalTimer))
	{
		return;
	}

	this->StopFiring();

	AActor* PlayerActor = GetOwner();
	if (!IsValid(PlayerActor))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::Reload !IsValid(PlayerActor)"));
		return;
	}

	AMultiplayerFPSCharacter* MultiplayerFPSPlayer = Cast<AMultiplayerFPSCharacter>(PlayerActor);
	if (!IsValid(MultiplayerFPSPlayer))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::Reload !IsValid(MultiplayerFPSPlayer)"));
		return;
	}

	FTimerDelegate AllowFiringDelegate;
	AllowFiringDelegate.BindUFunction(MultiplayerFPSPlayer, "SetIsReloading");

	MultiplayerFPSPlayer->SetIsReloading();

	GetWorldTimerManager().SetTimer(this->AllowFiringTimer, AllowFiringDelegate, this->ReloadTime, false);

	this->CurrentMagazineCapacity = this->MaxMagazineCapacity;
}

void AMultiplayerFPSFirearm::Zoom()
{
	UE_LOG(LogTemp, Warning, TEXT("Zoomed!"));

	AActor* PlayerActor = GetOwner();
	if (!IsValid(PlayerActor))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::Zoom !IsValid(PlayerActor)"));
		return;
	}

	AMultiplayerFPSCharacter* MultiplayerFPSPlayer = Cast<AMultiplayerFPSCharacter>(PlayerActor);
	if (!IsValid(MultiplayerFPSPlayer))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::Zoom !IsValid(MultiplayerFPSPlayer)"));
		return;
	}

	MultiplayerFPSPlayer->SetFOV(this->ZoomFOV);
}

void AMultiplayerFPSFirearm::ZoomOut()
{
	UE_LOG(LogTemp, Warning, TEXT("Zoomed Out!"));

	AActor* PlayerActor = GetOwner();
	if (!IsValid(PlayerActor))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::ZoomOut !IsValid(PlayerActor)"));
		return;
	}

	AMultiplayerFPSCharacter* MultiplayerFPSPlayer = Cast<AMultiplayerFPSCharacter>(PlayerActor);
	if (!IsValid(MultiplayerFPSPlayer))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSFirearm::ZoomOut !IsValid(MultiplayerFPSPlayer)"));
		return;
	}

	MultiplayerFPSPlayer->SetFOV(90.0f);
}