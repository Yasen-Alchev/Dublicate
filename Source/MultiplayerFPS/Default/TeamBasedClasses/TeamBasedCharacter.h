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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
	UMaterialInterface* TeamRedSkinTP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
	UMaterialInterface* TeamBlueSkinTP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
	UMaterialInterface* TeamNoneSkinTP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
	UMaterialInterface* TeamRedSkinFP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
	UMaterialInterface* TeamBlueSkinFP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
	UMaterialInterface* TeamNoneSkinFP;
	
	UPROPERTY()
	UMaterialInterface* ThirdPersonMaterial;

	UPROPERTY()
	UMaterialInterface* FirstPersonMaterial;

	UFUNCTION()
	virtual TEnumAsByte<ETeams> getTeam() { return Team; }

	UFUNCTION()
	virtual void InitTeam();

protected:
	UFUNCTION()
	virtual void MaterialChange();

};

