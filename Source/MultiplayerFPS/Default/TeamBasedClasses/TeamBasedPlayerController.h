#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerController.h"
#include "TeamBasedPlayerController.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ATeamBasedPlayerController : public AMultiplayerFPSPlayerController
{
	GENERATED_BODY()
	
public:

	ATeamBasedPlayerController();

	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	TEnumAsByte<ETeams> Team;
};

