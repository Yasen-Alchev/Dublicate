// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "MultiplayerFPS/Default/MultiplayerFPSGameInstance.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Kismet/GameplayStatics.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	numberOfGameModes = 3;
	gameModePos = 0;
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UMultiplayerFPSGameInstance* GameInstanceVar = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if(IsValid(GameInstanceVar))
	{
		if(GameInstanceVar->PlayerName.IsEmpty())
		{
			GoToLoginMenu();
		}
		else
		{
			GoToMainMenu();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> GameInstanceVar is not Valid !!!"));
	}

	if (IsValid(Button_QuitGame))
	{
		Button_QuitGame->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_QuitGame is not Valid !!!"));
	}

	if (IsValid(Button_LoginQuitGame))
	{
		Button_LoginQuitGame->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_LoginQuitGame is not Valid !!!"));
	}

	if (IsValid(Button_HostGame))
	{
		Button_HostGame->OnClicked.AddDynamic(this, &UMainMenuWidget::HostGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_HostGame is not Valid !!!"));
	}

	if (IsValid(Button_JoinGame))
	{
		Button_JoinGame->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToJoinMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_JoinGame is not Valid !!!"));
	}

	if (IsValid(ButtonLeft_ChooseGameMode))
	{
		ButtonLeft_ChooseGameMode->OnClicked.AddDynamic(this, &UMainMenuWidget::SwitchGameModeLeft);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> ButtonLeft_ChooseGameMode is not Valid !!!"));
	}

	if (IsValid(ButtonRight_ChooseGameMode))
	{
		ButtonRight_ChooseGameMode->OnClicked.AddDynamic(this, &UMainMenuWidget::SwitchGameModeRight);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> ButtonRight_ChooseGameMode is not Valid !!!"));
	}

	if (IsValid(Button_ConnectToServer))
	{
		Button_ConnectToServer->OnClicked.AddDynamic(this, &UMainMenuWidget::ConnectToServer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_ConnectToServer is not Valid !!!"));
	}

	if (IsValid(Button_BackToMainMenu))
	{
		Button_BackToMainMenu->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_BackToMainMenu is not Valid !!!"));
	}

	if (IsValid(Button_Login))
	{
		Button_Login->OnClicked.AddDynamic(this, &UMainMenuWidget::LoginUser);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_Login is not Valid !!!"));
	}

	if (IsValid(Button_LogOut))
	{
		Button_LogOut->OnClicked.AddDynamic(this, &UMainMenuWidget::LogOut);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_LogOut is not Valid !!!"));
	}

	if (IsValid(Button_Register))
	{
		Button_Register->OnClicked.AddDynamic(this, &UMainMenuWidget::RegisterUser);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_Register is not Valid !!!"));
	}

	if (IsValid(Button_SignUp))
	{
		Button_SignUp->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToRegisterMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_SignUp is not Valid !!!"));
	}

	if (IsValid(Button_BackToLogin))
	{
		Button_BackToLogin->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToLoginMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_BackToLogin is not Valid !!!"));
	}

	if (IsValid(Button_ResetPassword))
	{
		Button_ResetPassword->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToPasswordRecoveryMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_ResetPassword is not Valid !!!"));
	}

	if (IsValid(Button_PasswordRecoveryBackToLogin))
	{
		Button_PasswordRecoveryBackToLogin->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToLoginMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_PasswordRecoveryBackToLogin is not Valid !!!"));
	}

	if (IsValid(Button_PasswordSendRecoveryEmail))
	{
		Button_PasswordSendRecoveryEmail->OnClicked.AddDynamic(this, &UMainMenuWidget::RecoverPassword);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_PasswordSendRecoveryEmail is not Valid !!!"));
	}
}

void UMainMenuWidget::HostGame()
{
	UMultiplayerFPSGameInstance* GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if(IsValid(GameInstance))
	{
		const FString CaptureTheFlagLevel = "/Game/ThirdPersonCPP/Maps/CaptureTheFlagLevel";
		const FString DeathMatchLevel = "/Game/ThirdPersonCPP/Maps/DeathMatchLevel";
		const FString ConquestLevel = "/Game/ThirdPersonCPP/Maps/ConquestLevel";

		switch(gameModePos)
		{
			case 0:
				GameInstance->Host(CaptureTheFlagLevel);
				break;
			case 1:
				GameInstance->Host(DeathMatchLevel);
				break;
			case 2:
				GameInstance->Host(ConquestLevel);
				break;
			default:
				GameInstance->Host(CaptureTheFlagLevel);
				break;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::HostGame() -> GameInstance is not Valid !!!"));
	}
}

void UMainMenuWidget::ConnectToServer()
{
	UMultiplayerFPSGameInstance* GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (IsValid(GameInstance))
	{
		if(IsValid(EditableTextBox_ServerIP))
		{
			GameInstance->Join(EditableTextBox_ServerIP->GetText().ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::ConnectToServer() -> EditableTextBox_ServerIP is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::ConnectToServer() -> GameInstance is not Valid !!!"));
	}
}

void UMainMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMainMenuWidget::LoginUser()
{
	UMultiplayerFPSGameInstance* GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (IsValid(GameInstance))
	{
		if(IsValid(EditableTextBox_LoginUsername))
		{
			if (IsValid(EditableTextBox_LoginPassword))
			{
				GameInstance->LoginUser(EditableTextBox_LoginUsername->GetText().ToString(), EditableTextBox_LoginPassword->GetText().ToString(), this);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LoginUser() -> EditableTextBox_LoginPassword is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LoginUser() -> EditableTextBox_LoginUsername is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LoginUser() -> GameInstance is not Valid !!!"));
	}
}

void UMainMenuWidget::LogOut()
{
	UMultiplayerFPSGameInstance* GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (IsValid(GameInstance))
	{
		if (IsValid(EditableTextBox_LoginUsername))
		{
			if (IsValid(EditableTextBox_LoginPassword))
			{
				GameInstance->PlayerName = nullptr;
				EditableTextBox_LoginUsername->SetText(FText());
				EditableTextBox_LoginPassword->SetText(FText());
				GoToLoginMenu();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LogOut() -> EditableTextBox_LoginPassword is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LogOut() -> EditableTextBox_LoginUsername is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LogOut() -> GameInstance is not Valid !!!"));
	}
}

void UMainMenuWidget::RegisterUser()
{
	UMultiplayerFPSGameInstance* GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (IsValid(GameInstance))
	{
		if(IsValid(EditableTextBox_RegisterUsername))
		{
			if (IsValid(EditableTextBox_RegisterPassword))
			{
				if (IsValid(EditableTextBox_RegisterEmail))
				{
					GameInstance->RegisterUser(EditableTextBox_RegisterUsername->GetText().ToString(), 
						EditableTextBox_RegisterPassword->GetText().ToString(),
						EditableTextBox_RegisterEmail->GetText().ToString(),
						this
					);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::RegisterUser() -> EditableTextBox_RegisterEmail is not Valid !!!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::RegisterUser() -> EditableTextBox_RegisterPassword is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::RegisterUser() -> EditableTextBox_RegisterUsername is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LoginUser() -> GameInstance is not Valid !!!"));
	}
}

void UMainMenuWidget::RecoverPassword()
{
	UMultiplayerFPSGameInstance* GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (IsValid(GameInstance))
	{
		if (IsValid(EditableTextBox_RecoveryEmail))
		{
			GameInstance->RecoverPassword(EditableTextBox_RecoveryEmail->GetText().ToString(), this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::RecoverPassword() -> EditableTextBox_RecoveryEmail is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::RecoverPassword() -> GameInstance is not Valid !!!"));
	}
}

void UMainMenuWidget::GoToJoinMenu()
{
	if (IsValid(MenuSwitcher))
	{
		if(IsValid(JoinMenu))
		{
			MenuSwitcher->SetActiveWidget(JoinMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::JoinGame() -> JoinMenu is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::JoinGame() -> MenuSwitcher is not Valid !!!"));
	}
}

void UMainMenuWidget::GoToMainMenu()
{
	if (IsValid(MenuSwitcher))
	{
		if (IsValid(MainMenu))
		{
			MenuSwitcher->SetActiveWidget(MainMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToMainMenu() -> MainMenu is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToMainMenu() -> MenuSwitcher is not Valid !!!"));
	}
}

void UMainMenuWidget::GoToLoginMenu()
{
	if (IsValid(MenuSwitcher))
	{
		if (IsValid(LoginMenu))
		{
			MenuSwitcher->SetActiveWidget(LoginMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToLoginMenu() -> LoginMenu is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToLoginMenu() -> MenuSwitcher is not Valid !!!"));
	}
}

void UMainMenuWidget::GoToRegisterMenu()
{
	if (IsValid(MenuSwitcher))
	{
		if (IsValid(RegisterMenu))
		{
			MenuSwitcher->SetActiveWidget(RegisterMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToLoginMenu() -> RegisterMenu is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToLoginMenu() -> MenuSwitcher is not Valid !!!"));
	}
}

void UMainMenuWidget::GoToPasswordRecoveryMenu()
{
	if (IsValid(MenuSwitcher))
	{
		if (IsValid(PasswordRecoveryMenu))
		{
			MenuSwitcher->SetActiveWidget(PasswordRecoveryMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToPasswordRecoveryMenu() -> PasswordRecoveryMenu is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToPasswordRecoveryMenu() -> MenuSwitcher is not Valid !!!"));
	}
}

void UMainMenuWidget::SwitchGameModeLeft()
{
	if (IsValid(TXTBlock_GameModeText))
	{
		if(--gameModePos < 0)
		{
			gameModePos = 0;
		}
		else
		{
			TXTBlock_GameModeText->SetText(GetGameModePosText(gameModePos));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::SwitchGameModeLeft() -> TXTBlock_GameModeText is not Valid !!!"));
	}
}

void UMainMenuWidget::SwitchGameModeRight()
{
	if (IsValid(TXTBlock_GameModeText))
	{
		if (++gameModePos > (numberOfGameModes-1))
		{
			gameModePos = numberOfGameModes-1;
		}
		else
		{
			TXTBlock_GameModeText->SetText(GetGameModePosText(gameModePos));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::SwitchGameModeRight() -> TXTBlock_GameModeText is not Valid !!!"));
	}
}

FText UMainMenuWidget::GetGameModePosText(int pos)
{
	switch (pos)
	{
		case 0:
			return FText::FromString("Capture The Flag");
		case 1:
			return FText::FromString("Death match");
		case 2:
			return FText::FromString("Conquest");
		default:
			return FText::FromString("Capture The Flag");
	}
}
