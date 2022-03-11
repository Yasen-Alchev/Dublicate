#include "MultiplayerFPSFirearm.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "../Default/MultiplayerFPSCharacter.h"

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


}

void AMultiplayerFPSFirearm::BurstFire()
{

}

void AMultiplayerFPSFirearm::Fire()
{

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