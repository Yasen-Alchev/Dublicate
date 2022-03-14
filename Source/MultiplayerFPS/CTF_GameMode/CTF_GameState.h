#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/TeamBasedClasses/TeamBasedGameState.h"
#include "CTF_GameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API ACTF_GameState : public ATeamBasedGameState
{
	GENERATED_BODY()

public:
	ACTF_GameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn", meta = (ClampMin = "0", ClampMax = "40"))
	int FlagRespawnTime;


	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

};


