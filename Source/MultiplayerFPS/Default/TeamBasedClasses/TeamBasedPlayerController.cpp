#include "TeamBasedPlayerController.h"
#include "TeamBasedCharacter.h"

ATeamBasedPlayerController::ATeamBasedPlayerController(){}

void ATeamBasedPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATeamBasedPlayerController::OnPossess(APawn* MovieSceneBlends)
{
	Super::OnPossess(MovieSceneBlends);

	ATeamBasedCharacter* MyPawn = Cast<ATeamBasedCharacter>(MovieSceneBlends);
	if (IsValid(MyPawn))
	{
		MyPawn->InitTeam();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s ATeamBasedPlayerController::OnPossess(APawn* MovieSceneBlends) -> MyPawn is not Valid !!!"), *this->GetName());
	}
}


