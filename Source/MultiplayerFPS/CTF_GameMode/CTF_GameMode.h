// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/CommonClasses/Flag.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameMode.h"
#include "CTF_GameMode.generated.h"

UCLASS()
class ACTF_GameMode : public AMultiplayerFPSGameMode
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
		void setRedFlagTransform(FTransform FlagTransform) { RedFlagTransform = FlagTransform; }

	UFUNCTION()
		void setBlueFlagTransform(FTransform FlagTransform) { BlueFlagTransform = FlagTransform; }

    UFUNCTION()
		FTransform getRedFlagTransform() const { return RedFlagTransform; }

	UFUNCTION()
		FTransform getBlueFlagTransform() const { return BlueFlagTransform; }

    UFUNCTION()
		TSubclassOf<AFlag> getRedFlagBP() const { return this->RedFlagBP; }

    UFUNCTION()
		TSubclassOf<AFlag> getBlueFlagBP() const { return this->BlueFlagBP; }

    /* Flag's Properties END*/

private:

    UPROPERTY()
        FTransform RedFlagTransform;

    UPROPERTY()
        FTransform BlueFlagTransform;

};



