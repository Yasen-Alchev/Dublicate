#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSTeamBasedCharacter.h"
#include "CQ_Character.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACQ_Character : public AMultiplayerFPSTeamBasedCharacter
{
	GENERATED_BODY()

public:
	ACQ_Character();

protected:
	virtual void BeginPlay() override;

	
public:	
	virtual void Tick(float DeltaTime) override;

};