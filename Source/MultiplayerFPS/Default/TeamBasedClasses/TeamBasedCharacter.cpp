#include "TeamBasedCharacter.h"

#include "TeamBasedPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


ATeamBasedCharacter::ATeamBasedCharacter()
{
	this->ThirdPersonMaterial = nullptr;
	this->FirstPersonMaterial = nullptr;
	this->Team = TEAM_NONE;
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


void ATeamBasedCharacter::InitTeam()
{
	ATeamBasedPlayerState* PlayerStateVar = Cast<ATeamBasedPlayerState>(GetPlayerState());
	if (IsValid(PlayerStateVar))
	{
		this->Team = PlayerStateVar->Team;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s ATeamBasedCharacter::InitTeam(AActor * Player)->PlayerStateVar is not Valid !!!"), *PlayerName);
	}
	MaterialChange();
}

void ATeamBasedCharacter::MaterialChange()
{
	if (Team != TEAM_NONE)
	{
		if (Team == TEAM_BLUE)
		{
			this->ThirdPersonMaterial = this->FullBodyMesh->CreateDynamicMaterialInstance(0, this->TeamBlueSkinTP);
			this->FirstPersonMaterial = this->FirstPersonMesh->CreateDynamicMaterialInstance(0, this->TeamBlueSkinFP);
		}
		else
		{
			this->ThirdPersonMaterial = this->FullBodyMesh->CreateDynamicMaterialInstance(0, this->TeamRedSkinTP);
			this->FirstPersonMaterial = this->FirstPersonMesh->CreateDynamicMaterialInstance(0, this->TeamRedSkinFP);
		}
		if (IsValid(this->ThirdPersonMaterial) && IsValid(this->FirstPersonMaterial))
		{
			this->FullBodyMesh->SetMaterial(0, this->ThirdPersonMaterial);
			this->FirstPersonMesh->SetMaterial(0, this->FirstPersonMaterial);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s ATeamBasedCharacter::MaterialChange()-> MaterialInstance is not Valid!!!"), *PlayerName);
		}
	}
	else
	{
		this->ThirdPersonMaterial = this->FullBodyMesh->CreateDynamicMaterialInstance(0, this->TeamNoneSkinTP);
		this->FirstPersonMaterial = this->FirstPersonMesh->CreateDynamicMaterialInstance(0, this->TeamNoneSkinFP);

		if (IsValid(this->ThirdPersonMaterial) && IsValid(this->FirstPersonMaterial))
		{
			this->FullBodyMesh->SetMaterial(0, this->ThirdPersonMaterial);
			this->FirstPersonMesh->SetMaterial(0, this->FirstPersonMaterial);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s ATeamBasedCharacter::MaterialChange()-> MaterialInstance is not Valid!!!"), *PlayerName);
		}

		UE_LOG(LogTemp, Warning, TEXT("%s ATeamBasedCharacter::MaterialChange() -> Team is TEAM_NONE"), *PlayerName);
	}
}

