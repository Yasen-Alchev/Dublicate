#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Runtime/UMG/Public/UMG.h"
#include "WidgetUnits/BuyMenuWeaponUnitWidget.h"
#include "BuyMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UBuyMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UBuyMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION()
		virtual void BackToBuyMenu();

	UFUNCTION()
		virtual void ChooseSlotOne();

	UFUNCTION()
		virtual void ChooseSlotTwo();

	UFUNCTION()
		virtual void SelectWeapon(UBuyMenuWeaponUnitWidget* SelectedWeapon);

protected:

	UFUNCTION()
		virtual void ChooseSlot(uint32 slotIndex);

	UPROPERTY()
		UBuyMenuWeaponUnitWidget* SelectedWeaponRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_Slot1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_Slot2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_Back;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UScrollBox* BuyMenuScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UWidgetSwitcher* BuyMenuWidgetSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Image_Weapon;

	//Menus Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UWidget* BuyMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UWidget* ChooseSlotMenu;

};

