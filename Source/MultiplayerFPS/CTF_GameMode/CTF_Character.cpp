#include "CTF_Character.h"
#include "CTF_PlayerController.h"
#include "CTF_GameMode.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameInstance.h"
#include "Net/UnrealNetwork.h"

ACTF_Character::ACTF_Character()
{
	bHasFlag = false;
}

void ACTF_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ACTF_Character::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTF_Character, bHasFlag);
}

void ACTF_Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bDead)
	{
		Died();
	}
}

void ACTF_Character::Died()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		ACTF_PlayerController* PlayerController = Cast<ACTF_PlayerController>(Controller);
		if (IsValid(PlayerController))
		{
			if(bHasFlag)
			{
				DropFlag();
			}
			PlayerController->RespawnPlayer();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ACTF_Character::Died() -> PlayerController is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACTF_Character::Died() -> World is not Valid !!!"));
	}
}

void ACTF_Character::DropFlag()
{
	bHasFlag = false;
	ServerRPCDropFlag();
}

void ACTF_Character::ServerRPCDropFlag_Implementation()
{
	if(HasAuthority())
	{
		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			ACTF_GameMode* GameMode = Cast<ACTF_GameMode>(World->GetAuthGameMode());
			if (IsValid(GameMode))
			{
				FTransform FlagTransform;
				TSubclassOf<AFlag> BluePrintClass;
				FActorSpawnParameters SpawnParameters;

				FlagTransform = GetActorTransform();

				if(Team == TEAM_BLUE)
				{
					BluePrintClass = GameMode->getRedFlagBP();
				}
				else if(Team == TEAM_RED)
				{
					BluePrintClass = GameMode->getBlueFlagBP();
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("ACTF_Character::ServerRPCDropFlag_Implementation() -> Team is TEAM_NONE !!!"));
					return;
				}

				AFlag* Flag = World->SpawnActor<AFlag>(BluePrintClass, FlagTransform.GetLocation(), FlagTransform.GetRotation().Rotator(), SpawnParameters);
				Flag->bIsDropped = true;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ACTF_Character::ServerRPCDropFlag_Implementation() -> GameMode is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ACTF_Character::ServerRPCDropFlag_Implementation() -> World is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACTF_Character::ServerRPCDropFlag_Implementation() -> Does not have Authority !!!"));
	}
}

