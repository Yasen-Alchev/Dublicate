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

	SelectedWeaponRef = nullptr;

	if (!IsValid(Button_Back))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::NativeConstruct() -> Button_Back is not Valid !!!"));
		return;
	}
	Button_Back->OnClicked.AddDynamic(this, &UBuyMenuWidget::BackToBuyMenu);

	if (!IsValid(Button_Slot1))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::NativeConstruct() -> Button_Slot1 is not Valid !!!"));
		return;
	}
	Button_Slot1->OnClicked.AddDynamic(this, &UBuyMenuWidget::ChooseSlotOne);

	if (!IsValid(Button_Slot2))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::NativeConstruct() -> Button_Slot2 is not Valid !!!"));
		return;
	}
	Button_Slot2->OnClicked.AddDynamic(this, &UBuyMenuWidget::ChooseSlotTwo);
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
	if (!IsValid(BuyMenuWidgetSwitcher))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::BackToBuyMenu() -> BuyMenuWidgetSwitcher is not Valid !!!"));
		return;
	}

	if (!IsValid(BuyMenu))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::BackToBuyMenu() -> BuyMenu is not Valid !!!"));
		return;
	}

	SelectedWeaponRef = nullptr;
	BuyMenuWidgetSwitcher->SetActiveWidget(BuyMenu);
}

void UBuyMenuWidget::ChooseSlotOne()
{
	ChooseSlot(0);
}

void UBuyMenuWidget::ChooseSlotTwo()
{
	ChooseSlot(1);
}

void UBuyMenuWidget::SelectWeapon(UBuyMenuWeaponUnitWidget* SelectedWeapon)
{
	if (!IsValid(BuyMenuWidgetSwitcher))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::SelectWeapon() -> BuyMenuWidgetSwitcher is not Valid !!!"));
		return;
	}

	if (!IsValid(ChooseSlotMenu))
	{
		UE_LOG(LogTemp, Error, TEXT("UBuyMenuWidget::SelectWeapon() -> ChooseSlotMenu is not Valid !!!"));
		return;
	}

	SelectedWeaponRef = SelectedWeapon;
	Image_Weapon->SetBrush(SelectedWeapon->Image_Weapon->Brush);
	BuyMenuWidgetSwitcher->SetActiveWidget(ChooseSlotMenu);
}

void UBuyMenuWidget::ChooseSlot(uint32 slotIndex)
{
	if (!IsValid(SelectedWeaponRef))
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

	Character->ServerChangeWeapon(slotIndex, SelectedWeaponRef->WeaponClass);
	BackToBuyMenu();
	Character->ToggleBuyMenu();
}
