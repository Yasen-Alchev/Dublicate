#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameMode.h"
#include "TeamBasedGameMode.generated.h"

UCLASS(minimalapi)
class ATeamBasedGameMode : public AMultiplayerFPSGameMode
{
	GENERATED_BODY()

protected:

    ATeamBasedGameMode();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

    UPROPERTY()
        int redPlayers;

    UPROPERTY()
        int bluePlayers;

public:

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

    UFUNCTION()
        virtual TEnumAsByte<ETeams> assignTeam();

};



