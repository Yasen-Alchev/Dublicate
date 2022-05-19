#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/CommonClasses/Flag.h"
#include "MultiplayerFPS/Default/TeamBasedClasses/TeamBasedGameMode.h"
#include "CTF_GameMode.generated.h"

UCLASS()
class ACTF_GameMode : public ATeamBasedGameMode
{
	GENERATED_BODY()

public:
    ACTF_GameMode();

protected:

    virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
    
public:

    /* Flag's Properties */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlagSpawning")
	TSubclassOf<AFlag> RedFlagBP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlagSpawning")
	TSubclassOf<AFlag> BlueFlagBP;

    UFUNCTION()
		void SetRedFlagTransform(FTransform FlagTransform) { RedFlagTransform = FlagTransform; }

	UFUNCTION()
		void SetBlueFlagTransform(FTransform FlagTransform) { BlueFlagTransform = FlagTransform; }

    UFUNCTION()
		FTransform GetRedFlagTransform() const { return RedFlagTransform; }

	UFUNCTION()
		FTransform GetBlueFlagTransform() const { return BlueFlagTransform; }

    UFUNCTION()
		TSubclassOf<AFlag> GetRedFlagBP() const { return this->RedFlagBP; }

    UFUNCTION()
		TSubclassOf<AFlag> GetBlueFlagBP() const { return this->BlueFlagBP; }

    /* Flag's Properties END*/

private:

    UPROPERTY()
        FTransform RedFlagTransform;

    UPROPERTY()
        FTransform BlueFlagTransform;

};



