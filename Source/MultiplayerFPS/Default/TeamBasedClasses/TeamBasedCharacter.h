#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "MultiplayerFPS/Default/MultiplayerFPSCharacter.h"
#include "TeamBasedCharacter.generated.h"

UCLASS(config = Game)
class ATeamBasedCharacter : public AMultiplayerFPSCharacter
{
	GENERATED_BODY()

public:
	ATeamBasedCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Init() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
		UMaterialInterface* TeamRedSkin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
		UMaterialInterface* TeamBlueSkin;

	UPROPERTY()
		UMaterialInterface* MyMaterial;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
		TEnumAsByte<ETeams> Team;

	UFUNCTION()
		virtual TEnumAsByte<ETeams> GetTeam() { return Team; }

protected:

	UFUNCTION()
		virtual void MaterialChange();

};

