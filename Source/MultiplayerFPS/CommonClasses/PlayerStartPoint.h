#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerStartPoint.generated.h"

UCLASS()
class MULTIPLAYERFPS_API APlayerStartPoint : public APlayerStart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Defaults")
	TEnumAsByte<ETeams> Team;

};
