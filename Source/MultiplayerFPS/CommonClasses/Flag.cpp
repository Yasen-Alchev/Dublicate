#include "Flag.h"

#include "Kismet/GameplayStatics.h"
#include "MultiplayerFPS/CTF_GameMode/CTF_PlayerState.h"
#include "MultiplayerFPS/CTF_GameMode/CTF_Character.h"
#include "MultiplayerFPS/CTF_GameMode/CTF_GameMode.h"
#include "MultiplayerFPS/CTF_GameMode/CTF_GameState.h"

AFlag::AFlag()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent >(TEXT("Flag's Mesh"));
	RootComponent = MeshComponent;

	bIsDropped = false;
	RespawnTime = 3;
}

void AFlag::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AFlag::OnBeginOverlap);
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("AFlag::BeginPlay() -> World is not Valid !!!"));
		return;
	}

	ACTF_GameState* GameState = Cast<ACTF_GameState>(World->GetGameState());
	if (!IsValid(GameState))
	{
		UE_LOG(LogTemp, Error, TEXT("AFlag::BeginPlay() -> GameState is not Valid !!!"));
		return;
	}
	RespawnTime = GameState->FlagRespawnTime;
}

void AFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsDropped)
	{
		bIsDropped = false;
		GetWorldTimerManager().SetTimer(RespawnHandle, this, &AFlag::ServerRPCRespawnFlag, RespawnTime, false);
	}
}

void AFlag::OnBeginOverlap(class UPrimitiveComponent* HitComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("ACapturePoint::CheckStatus() -> World is not Valid!!!"));
		return;
	}

	ACTF_GameState* GameState = Cast<ACTF_GameState>(World->GetGameState());
	if (!IsValid(GameState))
	{
		UE_LOG(LogTemp, Error, TEXT("AFlag::OnBeginOverlap -> GameState is not Valid!!!"));
		return;
	}

	if (!GameState->IsGameStarted())
	{
		UE_LOG(LogTemp, Log, TEXT("Game was not started yet! "));
		return;
	}

	ACTF_Character* Player = Cast<ACTF_Character>(OtherActor);
	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Error, TEXT("FlagOverlap() -> Player is not Valid!!!"));
		return;
	}

	TEnumAsByte<ETeams> PlayerTeam = Player->Team;
	if (PlayerTeam != TEAM_NONE)
	{
		ServerRPCFlagOverlappedLogic(Player);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FlagOverlap() ->PlayerTeam is NONE!!!"));
	}
}

void AFlag::ServerRPCRespawnFlag_Implementation()
{
	if(!HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("AFlag::ServerRPCRespawnFlag_Implementation() -> Does not have Authority !!!"));
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("AFlag::ServerRPCRespawnFlag_Implementation() -> World is not Valid !!!"));
		return;
	}

	ACTF_GameMode* GameMode = Cast<ACTF_GameMode>(World->GetAuthGameMode());
	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Error, TEXT("AFlag::ServerRPCRespawnFlag_Implementation() -> GameMode is not Valid !!!"));
		return;
	}

	Destroy(true);
	FTransform FlagTransform;
	TSubclassOf<AFlag> BluePrintClass;
	FActorSpawnParameters SpawnParameters;

	if (FlagTeam == TEAM_RED)
	{
		BluePrintClass = GameMode->GetRedFlagBP();
		FlagTransform = GameMode->GetRedFlagTransform();
	}
	else if (FlagTeam == TEAM_BLUE)
	{
		BluePrintClass = GameMode->GetBlueFlagBP();
		FlagTransform = GameMode->GetBlueFlagTransform();
	}
	World->SpawnActor<AFlag>(BluePrintClass, FlagTransform.GetLocation(), FlagTransform.GetRotation().Rotator(), SpawnParameters);

}

void AFlag::ServerRPCFlagOverlappedLogic_Implementation(AActor* OtherActor)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("FlagOverlappedLogic_Implementation() -> Does not have Authority !!!"));
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("FlagOverlappedLogic_Implementation() -> World is not Valid !!!"));
		return;
	}

	ACTF_GameMode* GameMode = Cast<ACTF_GameMode>(World->GetAuthGameMode());
	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Error, TEXT("FlagOverlappedLogic_Implementation() -> GameMode is not Valid !!!"));
		return;
	}

	ACTF_GameState* GameState = Cast<ACTF_GameState>(GameMode->GameState);
	if (!IsValid(GameState))
	{
		UE_LOG(LogTemp, Error, TEXT("FlagOverlappedLogic_Implementation() -> GameState is not Valid !!!"));
		return;
	}

	ACTF_Character* Player = Cast<ACTF_Character>(OtherActor);
	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Error, TEXT("FlagOverlappedLogic_Implementation() -> Player is not Valid !!!"));
		return;
	}

	ACTF_PlayerState* PlayerState = Cast<ACTF_PlayerState>(Player->GetPlayerState());
	if (!IsValid(PlayerState))
	{
		UE_LOG(LogTemp, Error, TEXT("FlagOverlappedLogic_Implementation() -> PlayerState is not Valid !!!"));
		return;
	}

	TEnumAsByte<ETeams> PlayerTeam = Player->Team;
	if (FlagTeam != PlayerTeam)
	{
		if (!GetWorldTimerManager().IsTimerActive(RespawnHandle))
		{
			FTransform FlagTransform = GetActorTransform();

			if (FlagTeam == TEAM_NONE) UE_LOG(LogTemp, Error, TEXT("FlagOverlappedLogic_Implementation() -> FLAG TEAM WAS NOT SET!!!"));
			if (FlagTeam == TEAM_RED) GameMode->SetRedFlagTransform(FlagTransform);
			if (FlagTeam == TEAM_BLUE) GameMode->SetBlueFlagTransform(FlagTransform);
		}
		Player->bHasFlag = true;
		Destroy(true);
	}
	else if (Player->bHasFlag)
	{
		Player->bHasFlag = false;
		FTransform FlagTransform;
		TSubclassOf<AFlag> BluePrintClass;
		FActorSpawnParameters SpawnParameters;

		if (PlayerTeam == TEAM_BLUE)
		{
			BluePrintClass = GameMode->GetRedFlagBP();
			FlagTransform = GameMode->GetRedFlagTransform();
			GameState->BlueFlagCaptured();
		}
		else if (PlayerTeam == TEAM_RED)
		{
			BluePrintClass = GameMode->GetBlueFlagBP();
			FlagTransform = GameMode->GetBlueFlagTransform();
			GameState->RedFlagCaptured();
		}
		PlayerState->FlagWasCaptured();
		World->SpawnActor<AFlag>(BluePrintClass, FlagTransform.GetLocation(), FlagTransform.GetRotation().Rotator(), SpawnParameters);
		GameMode->UpdateObjectiveStats();
	}
}


