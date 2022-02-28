// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FPS_MultiplayCharacter.h"
#include "GameFramework/Character.h"
#include "FPS_MultiplayTeamBasedCharacter.generated.h"

UCLASS(config = Game)
class AFPS_MultiplayTeamBasedCharacter : public AFPS_MultiplayCharacter
{
	GENERATED_BODY()

public:
	AFPS_MultiplayTeamBasedCharacter();

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
		UMaterialInterface* TeamRedSkin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
		UMaterialInterface* TeamBlueSkin;

	UPROPERTY()
		UMaterialInterface* myMaterial;

	virtual void InitTeam() override;

	UPROPERTY(Replicated)
		int objectiveScore;

protected:

	UFUNCTION()
		virtual void MaterialChange();

};

