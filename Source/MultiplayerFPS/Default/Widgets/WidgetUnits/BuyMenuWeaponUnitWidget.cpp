// Fill out your copyright notice in the Description page of Project Settings.


#include "BuyMenuWeaponUnitWidget.h"

#include "MultiplayerFPS/Default/MultiplayerFPSCharacter.h"
#include "MultiplayerFPS/Default/MultiplayerFPSInGameHUD.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerController.h"
#include "MultiplayerFPS/Default/Widgets/BuyMenuWidget.h"


UBuyMenuWeaponUnitWidget::UBuyMenuWeaponUnitWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

void UBuyMenuWeaponUnitWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsValid(Button_SelectWeapon))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::NativeConstruct() -> Button_SelectWeapon is not Valid !!!"));
		return;
	}
	Button_SelectWeapon->OnClicked.AddDynamic(this, &UBuyMenuWeaponUnitWidget::SelectWeapon);
}

void UBuyMenuWeaponUnitWidget::SelectWeapon()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWeaponUnitWidget::SelectWeapon() -> World"));
		return;
	}

	AMultiplayerFPSPlayerController* Controller = Cast<AMultiplayerFPSPlayerController>(UGameplayStatics::GetPlayerController(World, 0));
	if(!IsValid(Controller))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWeaponUnitWidget::SelectWeapon() -> Controller"));
		return;
	}

	AMultiplayerFPSInGameHUD* PlayerHUD = Cast<AMultiplayerFPSInGameHUD>(Controller->GetHUD());
	if (!IsValid(PlayerHUD))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWeaponUnitWidget::SelectWeapon() -> PlayerHUD"));
		return;
	}

	PlayerHUD->SelectWeapon(this);
}
