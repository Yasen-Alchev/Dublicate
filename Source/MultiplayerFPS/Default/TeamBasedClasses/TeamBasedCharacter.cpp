#include "TeamBasedCharacter.h"

#include "TeamBasedPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


ATeamBasedCharacter::ATeamBasedCharacter()
{
	MyMaterial = nullptr;
	Team = TEAM_NONE;
}

void ATeamBasedCharacter::BeginPlay()
{
	Super::BeginPlay();

	MaterialChange();
}

void ATeamBasedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeamBasedCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamBasedCharacter, Team);
}


void ATeamBasedCharacter::Init()
{
	ATeamBasedPlayerState* PlayerStateVar = Cast<ATeamBasedPlayerState>(GetPlayerState());
	if (!IsValid(PlayerStateVar))
	{
		UE_LOG(LogTemp, Error, TEXT("ATeamBasedCharacter::Init(AActor * Player)->PlayerStateVar is not Valid !!!"));
		return;
	}
	this->Team = PlayerStateVar->Team;
	MaterialChange();

	Super::Init();
}

void ATeamBasedCharacter::MaterialChange()
{
	if (Team == TEAM_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATeamBasedCharacter::MaterialChange() -> Team is TEAM_NONE"));
		return;
	}

	if (Team == TEAM_BLUE)
	{
		MyMaterial = FullBodyMesh->CreateDynamicMaterialInstance(0, TeamBlueSkin);
	}
	else
	{
		MyMaterial = FullBodyMesh->CreateDynamicMaterialInstance(0, TeamRedSkin);
	}

	if (!IsValid(MyMaterial))
	{
		UE_LOG(LogTemp, Error, TEXT("ATeamBasedCharacter::MaterialChange()-> MaterialInstance is not Valid!!!"));
		return;
	}

	FullBodyMesh->SetMaterial(0, MyMaterial);
	FirstPersonMesh->SetMaterial(0, MyMaterial);
}

