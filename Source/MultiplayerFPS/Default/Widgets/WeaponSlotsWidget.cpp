// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSlotsWidget.h"

#include "MultiplayerFPS/Default/MultiplayerFPSCharacter.h"
#include "MultiplayerFPS/Default/MultiplayerFPSInGameHUD.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerController.h"

UWeaponSlotsWidget::UWeaponSlotsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){}

void UWeaponSlotsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponBorderSlot1->Background.DrawAs = ESlateBrushDrawType::Border;
	WeaponBorderSlot1->SetBrushFromTexture(BorderSelectorTexture);
	WeaponBorderSlot1->Background.Margin = 0;

	WeaponBorderSlot2->Background.DrawAs = ESlateBrushDrawType::Border;
	WeaponBorderSlot2->SetBrushFromTexture(BorderSelectorTexture);
	WeaponBorderSlot2->Background.Margin = 0;

	SelectWeaponSlot(0);

	AMultiplayerFPSPlayerController* Controller = Cast<AMultiplayerFPSPlayerController>(GetOwningPlayer());
	if (!IsValid(Controller))
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponSlotsWidget::NativeConstruct() -> Controller is not Valid !!!"));
		return;
	}

	AMultiplayerFPSCharacter* Player = Cast<AMultiplayerFPSCharacter>(Controller->GetPawn());
	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponSlotsWidget::NativeConstruct() -> Player is not Valid !!!"));
		return;
	}

	for (int32 i = 0; i < Player->FirearmArray.Num(); ++i)
	{
		if (IsValid(Player->FirearmArray[i]->WeaponTexture))
		{
			SetImageWeaponSlot(i, Player->FirearmArray[i]->WeaponTexture);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UWeaponSlotsWidget::NativeConstruct() !IsValid(FirearmArray[%d]->WeaponTexture)"), i);
		}
	}
}

void UWeaponSlotsWidget::SetImageWeaponSlot(uint32 SlotIndex, UTexture2D* WeaponTexture2D)
{
	if (!IsValid(WeaponTexture2D))
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponSlotsWidget::SetImageWeaponSlot() -> WeaponTexture2D is not Valid !!!"));
		return;
	}

	switch(SlotIndex)
	{
		case 0:
			SetImageWeaponSlot1(WeaponTexture2D);
			break;

		case 1:
			SetImageWeaponSlot2(WeaponTexture2D);
			break;

		default:
			UE_LOG(LogTemp, Error, TEXT("UWeaponSlotsWidget::SetImageWeaponSlot() -> Unhandled  SlotIndex Out of Bound !!!"));
			break;
	}
}

void UWeaponSlotsWidget::SetImageWeaponSlot1(UTexture2D* WeaponTexture2D)
{
	if (!IsValid(WeaponTexture2D))
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponSlotsWidget::SetImageWeaponSlot2 -> WeaponImage is not Valid !!!"));
		return;
	}

	WeaponImageSlot1->SetBrushFromTexture(WeaponTexture2D);
}

void UWeaponSlotsWidget::SetImageWeaponSlot2(UTexture2D* WeaponTexture2D)
{
	if(!IsValid(WeaponTexture2D))
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponSlotsWidget::SetImageWeaponSlot2 -> WeaponImage is not Valid !!!"));
		return;
	}
	WeaponImageSlot2->SetBrushFromTexture(WeaponTexture2D);

}

void UWeaponSlotsWidget::SelectWeaponSlot(uint32 SlotIndex)
{
	switch(SlotIndex)
	{
		case 0:
			WeaponBorderSlot1->Background.Margin = 1;
			WeaponBorderSlot2->Background.Margin = 0;
			break;

		case 1:
			WeaponBorderSlot1->Background.Margin = 0;
			WeaponBorderSlot2->Background.Margin = 1;
			break;

		default:
			UE_LOG(LogTemp, Error, TEXT("UWeaponSlotsWidget::SelectWeaponSlot() -> Unhandled  SlotIndex Out of Bound !!!"));
			break;
	}
}
