// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPS_Multiplay/FPS_CommonClasses/FPS_Flag.h"
#include "FPS_Multiplay/FPS_Default/FPS_MultiplayGameMode.h"
#include "CTF_GameMode.generated.h"

UCLASS()
class ACTF_GameMode : public AFPS_MultiplayGameMode
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
	TSubclassOf<AFPS_Flag> RedFlagBP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlagSpawning")
	TSubclassOf<AFPS_Flag> BlueFlagBP;

    UFUNCTION()
		void setRedFlagTransform(FTransform FlagTransform) { RedFlagTransform = FlagTransform; }

	UFUNCTION()
		void setBlueFlagTransform(FTransform FlagTransform) { BlueFlagTransform = FlagTransform; }

    UFUNCTION()
		FTransform getRedFlagTransform() const { return RedFlagTransform; }

	UFUNCTION()
		FTransform getBlueFlagTransform() const { return BlueFlagTransform; }

    UFUNCTION()
		TSubclassOf<AFPS_Flag> getRedFlagBP() const { return this->RedFlagBP; }

    UFUNCTION()
		TSubclassOf<AFPS_Flag> getBlueFlagBP() const { return this->BlueFlagBP; }

    /* Flag's Properties END*/

private:

    UPROPERTY()
        FTransform RedFlagTransform;

    UPROPERTY()
        FTransform BlueFlagTransform;

};



