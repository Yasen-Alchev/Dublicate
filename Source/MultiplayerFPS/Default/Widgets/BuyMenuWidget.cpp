#include "BuyMenuWidget.h"
#include "Input/Events.h"
#include "MultiplayerFPS/Default/MultiplayerFPSCharacter.h"

UBuyMenuWidget::UBuyMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UBuyMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedWeaponWidget = nullptr;

	if (IsValid(Button_Back))
	{
		Button_Back->OnClicked.AddDynamic(this, &UBuyMenuWidget::BackToBuyMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::NativeConstruct() -> Button_Back is not Valid !!!"));
	}

	if (IsValid(Button_Slot1))
	{
		Button_Slot1->OnClicked.AddDynamic(this, &UBuyMenuWidget::ChooseSlotOne);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::NativeConstruct() -> Button_Slot1 is not Valid !!!"));
	}

	if (IsValid(Button_Slot2))
	{
		Button_Slot2->OnClicked.AddDynamic(this, &UBuyMenuWidget::ChooseSlotTwo);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::NativeConstruct() -> Button_Slot2 is not Valid !!!"));
	}
}

FReply UBuyMenuWidget::NativeOnKeyDown(const FGeometry& MovieSceneBlends, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(MovieSceneBlends, InKeyEvent);

	FReply Reply = FReply::Handled();
	if (InKeyEvent.GetKey() == EKeys::B && !InKeyEvent.IsRepeat())
	{
		AMultiplayerFPSCharacter* Character = Cast<AMultiplayerFPSCharacter>(GetOwningPlayerPawn());
		if (!IsValid(Character))
		{
			UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::NativeOnKeyDown -> Character is not Valid !!!"));
			return Reply;
		}
		BackToBuyMenu();
		Character->ToggleBuyMenu();
	}
	return  Reply;
}

void UBuyMenuWidget::BackToBuyMenu()
{
	if (IsValid(BuyMenuWidgetSwitcher))
	{
		if (IsValid(BuyMenu))
		{
			SelectedWeaponWidget = nullptr;
			BuyMenuWidgetSwitcher->SetActiveWidget(BuyMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::BackToBuyMenu() -> BuyMenu is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::BackToBuyMenu() -> BuyMenuWidgetSwitcher is not Valid !!!"));
	}
}

void UBuyMenuWidget::ChooseSlotOne()
{
	ChooseSlot(0);
}

void UBuyMenuWidget::ChooseSlotTwo()
{
	ChooseSlot(1);
}

void UBuyMenuWidget::SelectWeapon(TSubclassOf<AMultiplayerFPSFirearm> WeaponClass)
{
	if (IsValid(BuyMenuWidgetSwitcher))
	{
		if (IsValid(ChooseSlotMenu))
		{
			SelectedWeaponWidget = WeaponClass;
			BuyMenuWidgetSwitcher->SetActiveWidget(ChooseSlotMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::SelectWeapon() -> ChooseSlotMenu is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::SelectWeapon() -> BuyMenuWidgetSwitcher is not Valid !!!"));
	}
}

void UBuyMenuWidget::ChooseSlot(uint32 slotIndex)
{
	if (!IsValid(SelectedWeaponWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::ChooseSlot(uint32 slotIndex) -> SelectedWeaponWidget is not Valid !!!"));
		return;
	}
	AMultiplayerFPSCharacter* Character = Cast<AMultiplayerFPSCharacter>(GetOwningPlayerPawn());
	if (!IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::ChooseSlot(uint32 slotIndex) -> Character is not Valid !!!"));
		return;
	}

	Character->ServerChangeWeapon(slotIndex, SelectedWeaponWidget);
	BackToBuyMenu();
	Character->ToggleBuyMenu();
}
