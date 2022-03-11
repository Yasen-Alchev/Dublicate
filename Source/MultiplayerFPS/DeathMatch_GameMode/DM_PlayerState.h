#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DM_PlayerState.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ADM_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADM_PlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ResetStats();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetKills() { return kills; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetDeaths() { return deaths; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int MadeAKill() { return ++kills; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int Died() { return ++deaths; }

private:

	UPROPERTY(Replicated)
	int kills;

	UPROPERTY(Replicated)
	int deaths;
};
