#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerState.h"
#include "TeamBasedPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API ATeamBasedPlayerState : public AMultiplayerFPSPlayerState
{
	GENERATED_BODY()

public:

	ATeamBasedPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Team")
		TEnumAsByte<ETeams> Team;

	UFUNCTION()
		virtual int GetCapturedFlagsCount() const { return FlagsCaptured; }

	UFUNCTION()
		virtual void FlagWasCaptured();

protected:

	UPROPERTY(ReplicatedUsing = ServerUpdateLeaderBoards)
		int FlagsCaptured;

	FTimerHandle TimerHandle;

	UFUNCTION(Server, Reliable)
		virtual void ServerUpdateLeaderBoards();
};

