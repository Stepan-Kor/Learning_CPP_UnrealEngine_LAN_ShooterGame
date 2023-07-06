// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"

bool UMainMenuWidget::Initialize()
{
	Super::Initialize();
	if (IsValid(ButtonExit))ButtonExit->OnClicked.AddDynamic(this, &UMainMenuWidget::Exit);
	//else GEngine->AddOnScreenDebugMessage(0, 5, FColor::Emerald, "Widget Have no button exit pointer");
	if (IsValid(ButtonRefreshList))ButtonRefreshList->OnClicked.AddDynamic(this, &UMainMenuWidget::RefreshList);
	if (IsValid(ButtonCreateServer))ButtonCreateServer->OnClicked.AddDynamic(this, &UMainMenuWidget::CreateServer);
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController)) { 		PlayerController->SetInputMode(FInputModeUIOnly()); 		PlayerController->SetShowMouseCursor(true);	}
	GameInstance = GetGameInstance<UShooterGameInstance>();
	if (GameInstance)GameInstance->SessionsSearchDelegateList.AddDynamic(this,&UMainMenuWidget::SessionsListUpdated);
	return true;
}

void UMainMenuWidget::Exit()
{	
	//UE_LOG(LogTemp,Warning,TEXT("Exit button clicked"))
	if (IsValid(PlayerController))	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
	//else GEngine->AddOnScreenDebugMessage(0,5,FColor::Emerald,"Widget Have no controller pointer");
}

void UMainMenuWidget::CreateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("button create server pressed"));
	FString ServerName;

	if (EditableTextBoxServerName)ServerName = EditableTextBoxServerName->GetText().ToString();
	if (ServerName == "")ServerName = DefaultSessionName;
	if (GameInstance)GameInstance->CreateSession(ServerName, DefaultHostName);
	else UE_LOG(LogTemp, Warning, TEXT("Widget have empty game instance pointer"));
}

void UMainMenuWidget::RefreshList()
{
	ButtonRefreshList->SetIsEnabled(false);
	TArray <UWidget*>  WidgetsToDelete = ScrollBoxList->GetAllChildren();
	for (UWidget* Widget : WidgetsToDelete) { 
		//Widget.NativeDestruct(); 
	}
	ScrollBoxList->ClearChildren();
	TextBlockRefreshList->SetText(FText::FromString("Searching for sessions . . ."));
	if (GameInstance)GameInstance->RefreshSessionsList();
}

void UMainMenuWidget::SessionsListUpdated(UShooterGameInstance* InstanceOfGame)
{
	TextBlockRefreshList->SetText(FText::FromString("Refresh list"));
	ButtonRefreshList->SetIsEnabled(true);
	UE_LOG(LogTemp, Warning, TEXT("Main menu widget got warning about sessions search done"));
	if (WidgetLineClass == nullptr) return;
	ScrollBoxList->ClearChildren();
	SearchResults = InstanceOfGame->SessionSearch->SearchResults;
	int32 TempInt = SearchResults.Num();

	UE_LOG(LogTemp, Warning, TEXT("Amount of Sessions found: %i"), TempInt);
	TempInt=0;
	int32 MaxPlayers = 5;
	int32 CurrentPlayers;
	FString TempString;
	USessionLineWidget* TempSessionDescription;
	for (FOnlineSessionSearchResult Result : SearchResults) {

		if (TempInt > 1000)	break;
		if (Result.IsValid()) {
			TempString = "Empty session name";
			Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), TempString);
			MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			CurrentPlayers = MaxPlayers - Result.Session.NumOpenPublicConnections+Result.Session.SessionSettings.bIsLANMatch;


			//Result.Session.SessionSettings.NumPublicConnections - Result.Session.NumOpenPublicConnections;

			//CurrentPlayers = MaxPlayers - Result.Session.NumOpenPrivateConnections+1;
			TempSessionDescription = CreateWidget<USessionLineWidget>(this, WidgetLineClass, FName("Session Line Widget"));
			TempSessionDescription->SetVariables(TempInt,TempString,CurrentPlayers,MaxPlayers);
			ScrollBoxList->AddChild(TempSessionDescription);
		}
		TempInt++;
	}
}