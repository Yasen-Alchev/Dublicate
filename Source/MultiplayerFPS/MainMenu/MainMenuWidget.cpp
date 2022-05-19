#include "MainMenuWidget.h"

#include "MultiplayerFPS/Default/MultiplayerFPSGameInstance.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Kismet/GameplayStatics.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NumberOfGameModes = 3;
	GameModePos = 0;
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UMultiplayerFPSGameInstance* GameInstanceVar = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if(!IsValid(GameInstanceVar))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> GameInstanceVar is not Valid !!!"));
		return;
	}

	if (GameInstanceVar->PlayerName.IsEmpty())
	{
		GoToLoginMenu();
	}
	else
	{
		GoToMainMenu();
	}

	if (!IsValid(Button_QuitGame))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_QuitGame is not Valid !!!"));
		return;
	}

	Button_QuitGame->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);

	if (!IsValid(Button_LoginQuitGame))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_LoginQuitGame is not Valid !!!"));
		return;
	}
	Button_LoginQuitGame->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);

	if (!IsValid(Button_HostGame))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_HostGame is not Valid !!!"));
		return;
	}
	Button_HostGame->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToHostMenu);

	if (!IsValid(Button_StartServer))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_StartServer is not Valid !!!"));
		return;
	}
	Button_StartServer->OnClicked.AddDynamic(this, &UMainMenuWidget::StartServer);

	if (!IsValid(Button_JoinGame))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_JoinGame is not Valid !!!"));
		return;
	}
	Button_JoinGame->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToJoinMenu);

	if (!IsValid(ButtonLeft_ChooseGameMode))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> ButtonLeft_ChooseGameMode is not Valid !!!"));
		return;
	}
	ButtonLeft_ChooseGameMode->OnClicked.AddDynamic(this, &UMainMenuWidget::SwitchGameModeLeft);

	if (!IsValid(ButtonRight_ChooseGameMode))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> ButtonRight_ChooseGameMode is not Valid !!!"));
		return;
	}
	ButtonRight_ChooseGameMode->OnClicked.AddDynamic(this, &UMainMenuWidget::SwitchGameModeRight);

	if (!IsValid(Button_ConnectToServer))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_ConnectToServer is not Valid !!!"));
		return;
	}
	Button_ConnectToServer->OnClicked.AddDynamic(this, &UMainMenuWidget::ConnectToServer);

	if (!IsValid(Button_BackToMainMenu))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_BackToMainMenu is not Valid !!!"));
		return;
	}
	Button_BackToMainMenu->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToMainMenu);

	if (!IsValid(Button_HostBackToMainMenu))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_HostBackToMainMenu is not Valid !!!"));
		return;
	}
	Button_HostBackToMainMenu->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToMainMenu);

	if (!IsValid(Button_Login))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_Login is not Valid !!!"));
		return;
	}
	Button_Login->OnClicked.AddDynamic(this, &UMainMenuWidget::LoginUser);

	if (!IsValid(Button_LogOut))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_LogOut is not Valid !!!"));
		return;
	}
	Button_LogOut->OnClicked.AddDynamic(this, &UMainMenuWidget::LogOut);

	if (!IsValid(Button_Register))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_Register is not Valid !!!"));
		return;
	}
	Button_Register->OnClicked.AddDynamic(this, &UMainMenuWidget::RegisterUser);

	if (!IsValid(Button_SignUp))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_SignUp is not Valid !!!"));
		return;
	}
	Button_SignUp->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToRegisterMenu);

	if (!IsValid(Button_BackToLogin))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_BackToLogin is not Valid !!!"));
		return;
	}
	Button_BackToLogin->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToLoginMenu);

	if (!IsValid(Button_ResetPassword))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_ResetPassword is not Valid !!!"));
		return;
	}
	Button_ResetPassword->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToPasswordRecoveryMenu);

	if (!IsValid(Button_PasswordRecoveryBackToLogin))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_PasswordRecoveryBackToLogin is not Valid !!!"));
		return;
	}
	Button_PasswordRecoveryBackToLogin->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToLoginMenu);

	if (!IsValid(Button_PasswordSendRecoveryEmail))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::NativeConstruct() -> Button_PasswordSendRecoveryEmail is not Valid !!!"));
		return;
	}
	Button_PasswordSendRecoveryEmail->OnClicked.AddDynamic(this, &UMainMenuWidget::RecoverPassword);
}

void UMainMenuWidget::StartServer()
{
	UMultiplayerFPSGameInstance* GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::HostGame() -> GameInstance is not Valid !!!"));
		return;
	}

	const FString CaptureTheFlagLevel = "/Game/ThirdPersonCPP/Maps/CaptureTheFlagLevel";
	const FString DeathMatchLevel = "/Game/ThirdPersonCPP/Maps/DeathMatchLevel";
	const FString ConquestLevel = "/Game/ThirdPersonCPP/Maps/ConquestLevel";

	switch (GameModePos)
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

void UMainMenuWidget::ConnectToServer()
{
	UMultiplayerFPSGameInstance* GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::ConnectToServer() -> GameInstance is not Valid !!!"));
		return;
	}

	if (!IsValid(EditableTextBox_JoinMenuServerIP))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::ConnectToServer() -> EditableTextBox_JoinMenuServerIP is not Valid !!!"));
		return;
	}

	if (!IsValid(EditableTextBox_JoinMenuServerPort))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::ConnectToServer() -> EditableTextBox_JoinMenuServerPort is not Valid !!!"));
		return;
	}

	GameInstance->Join(EditableTextBox_JoinMenuServerIP->GetText().ToString(), EditableTextBox_JoinMenuServerPort->GetText().ToString());

}

void UMainMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMainMenuWidget::LoginUser()
{
	UMultiplayerFPSGameInstance* GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LoginUser() -> GameInstance is not Valid !!!"));
		return;
	}

	if (!IsValid(EditableTextBox_LoginUsername))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LoginUser() -> EditableTextBox_LoginUsername is not Valid !!!"));
		return;
	}

	if (!IsValid(EditableTextBox_LoginPassword))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LoginUser() -> EditableTextBox_LoginPassword is not Valid !!!"));
		return;
	}
	GameInstance->LoginUser(EditableTextBox_LoginUsername->GetText().ToString(), EditableTextBox_LoginPassword->GetText().ToString(), this);

}

void UMainMenuWidget::LogOut()
{
	UMultiplayerFPSGameInstance* GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LogOut() -> GameInstance is not Valid !!!"));
		return;
	}

	if (!IsValid(EditableTextBox_LoginUsername))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LogOut() -> EditableTextBox_LoginUsername is not Valid !!!"));
		return;
	}

	if (!IsValid(EditableTextBox_LoginPassword))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LogOut() -> EditableTextBox_LoginPassword is not Valid !!!"));
		return;
	}

	GameInstance->PlayerName = nullptr;
	EditableTextBox_LoginUsername->SetText(FText());
	EditableTextBox_LoginPassword->SetText(FText());
	GoToLoginMenu();

}

void UMainMenuWidget::RegisterUser()
{
	UMultiplayerFPSGameInstance* GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::LoginUser() -> GameInstance is not Valid !!!"));
		return;
	}

	if (!IsValid(EditableTextBox_RegisterUsername))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::RegisterUser() -> EditableTextBox_RegisterUsername is not Valid !!!"));
		return;
	}

	if (!IsValid(EditableTextBox_RegisterPassword))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::RegisterUser() -> EditableTextBox_RegisterPassword is not Valid !!!"));
		return;
	}

	if (!IsValid(EditableTextBox_RegisterEmail))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::RegisterUser() -> EditableTextBox_RegisterEmail is not Valid !!!"));
		return;
	}

	GameInstance->RegisterUser(EditableTextBox_RegisterUsername->GetText().ToString(),
		EditableTextBox_RegisterPassword->GetText().ToString(),
		EditableTextBox_RegisterEmail->GetText().ToString(),
		this
	);
}

void UMainMenuWidget::RecoverPassword()
{
	UMultiplayerFPSGameInstance* GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::RecoverPassword() -> GameInstance is not Valid !!!"));
		return;
	}

	if (!IsValid(EditableTextBox_RecoveryEmail))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::RecoverPassword() -> EditableTextBox_RecoveryEmail is not Valid !!!"));
		return;
	}
	GameInstance->RecoverPassword(EditableTextBox_RecoveryEmail->GetText().ToString(), this);

}

void UMainMenuWidget::GoToHostMenu()
{
	if (!IsValid(MenuSwitcher))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::JoinGame() -> MenuSwitcher is not Valid !!!"));
		return;
	}

	if (!IsValid(HostMenu))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::JoinGame() -> HostMenu is not Valid !!!"));
		return;
	}
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenuWidget::GoToJoinMenu()
{
	if (!IsValid(MenuSwitcher))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::JoinGame() -> MenuSwitcher is not Valid !!!"));
		return;
	}

	if (!IsValid(JoinMenu))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::JoinGame() -> JoinMenu is not Valid !!!"));
		return;
	}
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenuWidget::GoToMainMenu()
{
	if (!IsValid(MenuSwitcher))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToMainMenu() -> MenuSwitcher is not Valid !!!"));
		return;
	}

	if (!IsValid(MainMenu))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToMainMenu() -> MainMenu is not Valid !!!"));
		return;
	}
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenuWidget::GoToLoginMenu()
{
	if (!IsValid(MenuSwitcher))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToLoginMenu() -> MenuSwitcher is not Valid !!!"));
		return;
	}

	if (!IsValid(LoginMenu))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToLoginMenu() -> LoginMenu is not Valid !!!"));
		return;
	}
	MenuSwitcher->SetActiveWidget(LoginMenu);
}

void UMainMenuWidget::GoToRegisterMenu()
{
	if (!IsValid(MenuSwitcher))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToLoginMenu() -> MenuSwitcher is not Valid !!!"));
		return; 
	}

	if (!IsValid(RegisterMenu))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToLoginMenu() -> RegisterMenu is not Valid !!!"));
		return;
	}
	MenuSwitcher->SetActiveWidget(RegisterMenu);
}

void UMainMenuWidget::GoToPasswordRecoveryMenu()
{
	if (!IsValid(MenuSwitcher))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToPasswordRecoveryMenu() -> MenuSwitcher is not Valid !!!"));
		return;
	}

	if (!IsValid(PasswordRecoveryMenu))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::GoToPasswordRecoveryMenu() -> PasswordRecoveryMenu is not Valid !!!"));
		return;
	}
	MenuSwitcher->SetActiveWidget(PasswordRecoveryMenu);
}

void UMainMenuWidget::SwitchGameModeLeft()
{
	if (!IsValid(TXTBlock_GameModeText))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::SwitchGameModeLeft() -> TXTBlock_GameModeText is not Valid !!!"));
		return;
	}

	if (--GameModePos <= 0)
	{
		GameModePos = NumberOfGameModes;
	}
	TXTBlock_GameModeText->SetText(GetGameModePosText(GameModePos));

}

void UMainMenuWidget::SwitchGameModeRight()
{
	if (!IsValid(TXTBlock_GameModeText))
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenuWidget::SwitchGameModeRight() -> TXTBlock_GameModeText is not Valid !!!"));
		return;
	}

	if (++GameModePos > (NumberOfGameModes - 1))
	{
		GameModePos = 0;
	}
	TXTBlock_GameModeText->SetText(GetGameModePosText(GameModePos));
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
