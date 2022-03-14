#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MultiplayerFPS/Default/TeamBasedClasses/TeamBasedCharacter.h"
#include "CTF_Character.generated.h"

UCLASS(config=Game)
class ACTF_Character : public ATeamBasedCharacter
{
	GENERATED_BODY()

public:
	ACTF_Character();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly)
		bool bHasFlag;

	UFUNCTION()
		void Died();

	UFUNCTION()
		void DropFlag();

private:

	UFUNCTION(Server, Reliable)
		void ServerRPCDropFlag();

};


