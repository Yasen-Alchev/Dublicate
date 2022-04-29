// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/MultiplayerFPSFirearm.h"
#include "Runtime/UMG/Public/UMG.h"
#include "BuyMenuWeaponUnitWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UBuyMenuWeaponUnitWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UBuyMenuWeaponUnitWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		void SelectWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Image_Weapon;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AMultiplayerFPSFirearm> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_SelectWeapon;
};
