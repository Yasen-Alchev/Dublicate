// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_CapturePoint.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "FPS_Multiplay/Conquest_GameMode/CQ_Character.h"
#include "FPS_Multiplay/Conquest_GameMode/CQ_GameState.h"
#include "FPS_Multiplay/Conquest_GameMode/CQ_PlayerState.h"
#include "FPS_Multiplay/CTF_GameMode/CTF_Character.h"
#include "FPS_Multiplay/CTF_GameMode/CTF_GameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPS_CapturePoint::AFPS_CapturePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	PlatformMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Capture Point Platform Mesh"));
	RodMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Capture Point Rod Mesh"));
	FlagMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Capture Point Flag Mesh"));
	CapturePointDetectionRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Capture Point Sphere Collision Detection Range"));

	FlagMeshComponent->SetIsReplicated(true);

	RootComponent = PlatformMeshComponent;
	RodMeshComponent->SetupAttachment(PlatformMeshComponent);
	FlagMeshComponent->SetupAttachment(PlatformMeshComponent);
	CapturePointDetectionRangeSphere->SetupAttachment(PlatformMeshComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (CubeMesh.Succeeded()) {
		PlatformMeshComponent->SetStaticMesh(CubeMesh.Object);
		RodMeshComponent->SetStaticMesh(CubeMesh.Object);
		FlagMeshComponent->SetStaticMesh(CubeMesh.Object);

		PlatformMeshComponent->SetMobility(EComponentMobility::Stationary);
		PlatformMeshComponent->SetRelativeScale3D(FVector(3, 3, 0.2));

		RodMeshComponent->SetMobility(EComponentMobility::Stationary);
		RodMeshComponent->SetRelativeLocation(FVector(0, 0, 950));
		RodMeshComponent->SetRelativeScale3D(FVector(0.05, 0.05, 18));

		FlagMeshComponent->SetMobility(EComponentMobility::Movable);
		FlagMeshComponent->SetRelativeLocation(FlagBottomLocation);
		FlagMeshComponent->SetRelativeScale3D(FVector(0.02, 0.37, 3));
		FlagMeshComponent->SetCollisionProfileName(FName("NoCollision"));
	}
	CapturePointDetectionRangeSphere->SetRelativeScale3D(FVector(65, 65, 65));
	CapturePointDetectionRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AFPS_CapturePoint::OnBeginOverlap);
	CapturePointDetectionRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AFPS_CapturePoint::OnEndOverlap);
}

void AFPS_CapturePoint::TimelineProgress(float Value)
{
	bIsCaptured = false;
	bIsFlagBottomPosition = false;

	if(FMath::IsNearlyZero(Value, 0.00001f))
	{
		bIsFlagBottomPosition = true;
	}
	else if(Value == 1)
	{
		bIsCaptured = true;
	}

	CheckStatus();

	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
	FlagMeshComponent->SetRelativeLocation(NewLocation);
}

// Called when the game starts or when spawned
void AFPS_CapturePoint::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
	CurveTimeLine.AddInterpFloat(CurveFloat, TimelineProgress);
	CurveTimeLine.SetLooping(false);

	StartLocation = FlagBottomLocation;
	EndLocation = FlagTopLocation;

	ShowFlag(false);
	ChangeCapturePointColor(NeutralSkin, true);
	CapturePointTeam = TEAM_NONE;
	bIsFlagBottomPosition = false;
	bIsCaptured = false;
	bClaimed = false;
}

void AFPS_CapturePoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPS_CapturePoint, bIsCaptured);
	DOREPLIFETIME(AFPS_CapturePoint, CurveTimeLine);
	DOREPLIFETIME(AFPS_CapturePoint, CapturePointTeam);
}

// Called every frame
void AFPS_CapturePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurveTimeLine.TickTimeline(DeltaTime);
}

void AFPS_CapturePoint::ChangeCapturePointColor(UMaterialInterface* Material, bool ChangeFlagColor)
{
	if (IsValid(Material))
	{
		myMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (IsValid(myMaterial))
		{
			if (ChangeFlagColor)
			{
				FlagMeshComponent->SetMaterial(0, myMaterial);
			}
			PlatformMeshComponent->SetMaterial(0, myMaterial);
			RodMeshComponent->SetMaterial(0, myMaterial);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_CapturePoint::ChangeCapturePointColor(UMaterialInterface* Material, bool ChangeFlagColor) -> Material is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_CapturePoint::ChangeCapturePointColor(UMaterialInterface* Material, bool ChangeFlagColor) -> Material is not Valid!!!"));
	}
}

void AFPS_CapturePoint::ChangeFlagColor(UMaterialInterface* Material)
{
	if (IsValid(Material))
	{
		myMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (IsValid(myMaterial))
		{
			FlagMeshComponent->SetMaterial(0, myMaterial);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_CapturePoint::ChangeFlagColor(UMaterialInterface* Material) -> myMaterial is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_CapturePoint::ChangeFlagColor(UMaterialInterface* Material) -> Material is not Valid!!!"));
	}
}

void AFPS_CapturePoint::ShowFlag(bool bIsVisible)
{
	if(FlagMeshComponent->IsVisible() && !bIsVisible)
	{
		FlagMeshComponent->SetVisibility(false);
	}
	else if(!FlagMeshComponent->IsVisible() && bIsVisible)
	{
		FlagMeshComponent->SetVisibility(true);
	}
}

void AFPS_CapturePoint::UpdateObjectiveStats()
{
	UE_LOG(LogTemp, Error, TEXT("AFPS_CapturePoint::UpdateObjectiveStats() -> Capture point"));
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		ACQ_GameState* GameStateVar = Cast<ACQ_GameState>(World->GetGameState());
		if (IsValid(GameStateVar))
		{
			GameStateVar->UpdateObjectiveStats();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_CapturePoint::UpdateObjectiveStats() -> GameModeVar is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_CapturePoint::UpdateObjectiveStats() -> World is not Valid!!!"));
	}
}

void AFPS_CapturePoint::MoveFlagUp()
{
	if (CurveFloat)
	{
		CurveTimeLine.Play();
	}
}

void AFPS_CapturePoint::MoveFlagDown()
{
	if (CurveFloat)
	{
		CurveTimeLine.Reverse();
	}
}

void AFPS_CapturePoint::CheckStatus()
{
	TArray<AActor*> Players;
	TArray<ACQ_Character*> CQ_Players;
	CapturePointDetectionRangeSphere->GetOverlappingActors(Players, ACQ_Character::StaticClass());

	int red = 0;
	int blue = 0;

	for (auto PlayerPtr : Players)
	{
		ACQ_Character* Player = Cast<ACQ_Character>(PlayerPtr);
		if (IsValid(Player))
		{
			CQ_Players.Add(Player);
			if (Player->getTeam() == TEAM_BLUE) blue++;
			else if (Player->getTeam() == TEAM_RED) red++;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_CapturePoint::CheckStatus() -> Player is not Valid!!!"));
		}
	}
	
	if(red == blue)
	{
		CurveTimeLine.Stop();
	}
	else if(red > blue)
	{
		switch (CapturePointTeam)
		{
			case TEAM_BLUE:
				if(bIsFlagBottomPosition)
				{
					MoveFlagUp();
					ChangeFlagColor(TeamRedSkin);
					CapturePointTeam = TEAM_RED;
				}
				else if(bIsCaptured)
				{
					bClaimed = false;
					MoveFlagDown();
					ChangeCapturePointColor(NeutralSkin);
				}
				else
				{
					MoveFlagDown();
				}
				break;
			case TEAM_RED:
				if (bIsCaptured && !bClaimed)
				{
					bClaimed = true;
					ChangeCapturePointColor(TeamRedSkin);
					for(auto Player: CQ_Players)
					{
						ACQ_PlayerState* PlayerStateVar = Cast<ACQ_PlayerState>(Player->GetPlayerState());
						if(IsValid(PlayerStateVar))
						{
							PlayerStateVar->FlagWasCaptured();
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("AFPS_CapturePoint::CheckStatus() -> PlayerStateVar is not Valid!!!"));
						}
					}
					//UpdateObjectiveStats();

					UWorld* World = GetWorld();
					if (IsValid(World))
					{
						AFPS_MultiplayGameState* GameStateVar = Cast<AFPS_MultiplayGameState>(World->GetGameState());
						if (IsValid(GameStateVar))
						{
							GameStateVar->RedFlagCaptured();
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerState::UpdateStat() -> GameStateVar is not Valid !!!"));
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerState::UpdateStat() -> World is not Valid !!!"));
					}

				}
				else
				{
					MoveFlagUp();
				}
				break;
			case TEAM_NONE:
				MoveFlagUp();
				ChangeFlagColor(TeamRedSkin);
				CapturePointTeam = TEAM_RED;
				break;
		}
	}
	else if (blue > red)
	{
		switch (CapturePointTeam)
		{
			case TEAM_BLUE:
				if (bIsCaptured && !bClaimed)
				{
					bClaimed = true;
					ChangeCapturePointColor(TeamBlueSkin);
					for (auto Player : CQ_Players)
					{
						ACQ_PlayerState* PlayerStateVar = Cast<ACQ_PlayerState>(Player->GetPlayerState());
						if (IsValid(PlayerStateVar))
						{
							PlayerStateVar->FlagWasCaptured();
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("AFPS_CapturePoint::CheckStatus() -> PlayerStateVar is not Valid!!!"));
						}
					}
					//UpdateObjectiveStats();

					UWorld* World = GetWorld();
					if (IsValid(World))
					{
						AFPS_MultiplayGameState* GameStateVar = Cast<AFPS_MultiplayGameState>(World->GetGameState());
						if (IsValid(GameStateVar))
						{
							GameStateVar->BlueFlagCaptured();
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerState::UpdateStat() -> GameStateVar is not Valid !!!"));
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerState::UpdateStat() -> World is not Valid !!!"));
					}

				}
				else
				{
					MoveFlagUp();
				}
				break;
			case TEAM_RED:
				if(bIsFlagBottomPosition)
				{
					MoveFlagUp();
					ChangeFlagColor(TeamBlueSkin);
					CapturePointTeam = TEAM_BLUE;
				}
				else if(bIsCaptured)
				{
					bClaimed = false;
					MoveFlagDown();
					ChangeCapturePointColor(NeutralSkin);
				}
				else
				{
					MoveFlagDown();
				}
				break;
			case TEAM_NONE:
				MoveFlagUp();
				ChangeFlagColor(TeamBlueSkin);
				CapturePointTeam = TEAM_BLUE;
				break;
		}
	}
}

void AFPS_CapturePoint::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ShowFlag(true);
	CheckStatus();
}

void AFPS_CapturePoint::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckStatus();
}
