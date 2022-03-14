#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/TeamBasedClasses/TeamBasedCharacter.h"
#include "CQ_Character.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACQ_Character : public ATeamBasedCharacter
{
	GENERATED_BODY()

public:
	ACQ_Character();

protected:
	virtual void BeginPlay() override;

	
public:	
	virtual void Tick(float DeltaTime) override;

};