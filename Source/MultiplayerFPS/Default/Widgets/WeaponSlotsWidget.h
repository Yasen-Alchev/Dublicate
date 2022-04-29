// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "WeaponSlotsWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UWeaponSlotsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWeaponSlotsWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
		virtual void NativeConstruct() override;

	UFUNCTION()
		virtual void SetImageWeaponSlot(uint32 SlotIndex, UTexture2D* WeaponTexture2D);

	UFUNCTION()
		virtual void SelectWeaponSlot(uint32 SlotIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* BorderSelectorTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UBorder* WeaponBorderSlot1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UBorder* WeaponBorderSlot2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* WeaponImageSlot1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* WeaponImageSlot2;

private:

	uint32 ActiveSlotIndex = 0;

	UFUNCTION()
		virtual void SetImageWeaponSlot1(UTexture2D* WeaponTexture2D);

	UFUNCTION()
		virtual void SetImageWeaponSlot2(UTexture2D* WeaponTexture2D);

};
