#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPSCharacter.h"
#include "GameFramework/Character.h"
#include "MultiplayerFPSTeamBasedCharacter.generated.h"

UCLASS(config = Game)
class AMultiplayerFPSTeamBasedCharacter : public AMultiplayerFPSCharacter
{
	GENERATED_BODY()

public:
	AMultiplayerFPSTeamBasedCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
		UMaterialInterface* TeamRedSkin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
		UMaterialInterface* TeamBlueSkin;

	UPROPERTY()
		UMaterialInterface* myMaterial;

	virtual void InitTeam() override;

protected:

	UFUNCTION()
		virtual void MaterialChange();

};

