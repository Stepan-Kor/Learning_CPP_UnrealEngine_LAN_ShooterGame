// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterWidget.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerState.h"
#include "ShooterGameInstance.h"
#include "Kismet/GameplayStatics.h"


bool UShooterWidget::Initialize()
{
	Super::Initialize();
	
	if (IsValid(Button_Restart))		Button_Restart->OnClicked.AddDynamic(this, &UShooterWidget::RestartGame);
	if (IsValid(Button_Exit))		Button_Exit->OnClicked.AddDynamic(this, &UShooterWidget::ExitGame);
	if (IsValid(Button_MainMenu))		Button_MainMenu->OnClicked.AddDynamic(this, &UShooterWidget::BackToMainMenu);
	if (IsValid(Button_Continue))		Button_Continue->OnClicked.AddDynamic(this, &UShooterWidget::Continue);
	GameInstance = Cast<UShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	return false;
}
void UShooterWidget::SetNewPercentage(float Amount)
{
	if (IsValid(ProgressBar)) ProgressBar->SetPercent(Amount);
}

void UShooterWidget::GameFinished(bool Success)
{
	//UE_LOG(LogTemp, Warning, TEXT("screen widget game finished %d"), Success);

	Button_Continue->SetVisibility(ESlateVisibility::Hidden);
	if(IsValid(VerticalBox_Menus))VerticalBox_Menus->SetVisibility(ESlateVisibility::Visible);
	else		UE_LOG(LogTemp, Warning, TEXT("Screen widget have no pointer to vertical box"));
	FText TempText = FText::FromString(Success ? "Victory ! ! !" : "Game over . . .");
	if(IsValid(TextBlock_EndType))TextBlock_EndType->SetText(TempText);
	else		UE_LOG(LogTemp, Warning, TEXT("Screen widget have no pointer to text block finished type"));
}

void UShooterWidget::OpenPauseMenu(bool WithPause)
{
	if (PlController) {
		//if (PlController->HasAuthority())PlController->ServerPause();
		//if(PlController->HasAuthority())UGameplayStatics::SetGamePaused(GetWorld(), true);
		//if(PlState)PlState->Multi_PauseAllClients(true);
		if (WithPause) {
			if (PlState)PlState->PauseSession(true);
			else {
				if (PlController)PlState = PlController->GetPlayerState<AShooterPlayerState>();
				if (PlState)PlState->PauseSession(true);
				else	UE_LOG(LogTemp, Warning, TEXT("Screen widget have no pointer to Player State."));
			}
		}
		PlController->SetInputMode(FInputModeUIOnly());
		PlController->SetShowMouseCursor(true);
		Button_Continue->SetVisibility(ESlateVisibility::Visible);
		if (IsValid(TextBlock_EndType))TextBlock_EndType->SetText(FText::FromString("Pause"));
		else		UE_LOG(LogTemp, Warning, TEXT("Screen widget have no pointer to text block finished type"));
		if (IsValid(VerticalBox_Menus))VerticalBox_Menus->SetVisibility(ESlateVisibility::Visible);
		else		UE_LOG(LogTemp, Warning, TEXT("Screen widget have no pointer to vertical box"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("Screen widget %s have no pointer Controller"), *this->GetName());
}

void UShooterWidget::Continue()
{
	if (!IsValid(PlController))return;
	VerticalBox_Menus->SetVisibility(ESlateVisibility::Hidden);
	PlController->SetInputMode(FInputModeGameOnly());
	PlController->SetShowMouseCursor(false);
	//if (PlController->HasAuthority())UGameplayStatics::SetGamePaused(GetWorld(),false);
	//if (IsValid(PlState))PlState->Multi_PauseAllClients(false);
	if (PlState)PlState->PauseSession(false);
	else UE_LOG(LogTemp, Warning, TEXT("Screen widget have not Player State."));
	PlState = PlController->GetPlayerState<AShooterPlayerState>();
	if (PlState)PlState->PauseSession(false);
}

void UShooterWidget::BackToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(),  "/Game/MainMenuLevel", true);
}

void UShooterWidget::HideMainMenu()
{
	VerticalBox_Menus->SetVisibility(ESlateVisibility::Hidden);
	if (!IsValid(PlController))return;
	PlController->SetInputMode(FInputModeGameOnly());
	PlController->SetShowMouseCursor(false);
}


void UShooterWidget::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),UGameplayStatics::GetPlayerController(GetWorld(),0),EQuitPreference::Quit,true);
}

void UShooterWidget::RestartGame()
{
	bool bHaveAuthority = PlController->HasAuthority();
	if (GameInstance && PlController) {
		if (GameInstance->IsCurrentlyInSession() && bHaveAuthority) {
			Continue();
			GetWorld()->ServerTravel("/Game/FirstPersonCPP/Maps/FirstPersonExampleMap?Listen");
		}
		else if (bHaveAuthority) { 
			UE_LOG(LogTemp, Warning, TEXT("Screen widget: player not in session."));
			UGameplayStatics::OpenLevel(GetWorld(), *GetWorld()->GetName(), true);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Screen widget dont have instance or controller"))
		if (GetWorld()->GetFirstPlayerController()->HasAuthority())
			UGameplayStatics::OpenLevel(GetWorld(), *GetWorld()->GetName(), true);
	}
}

void UShooterWidget::SetController(APlayerController* NewController)
{
	if (NewController) {
		PlController = NewController;
		Button_Restart->SetIsEnabled(PlController->HasAuthority());
		PlState = PlController->GetPlayerState<AShooterPlayerState>();
		if(!IsValid(PlState)) UE_LOG(LogTemp, Warning, TEXT("Screen widget could not get Player State."));
	}
	else UE_LOG(LogTemp, Warning, TEXT("Screen widget was send empty controller pointer"));
}
