// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionLineWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameInstance.h"

bool USessionLineWidget::Initialize()
{
	Super::Initialize();
	if(ButtonJoin)ButtonJoin->OnClicked.AddDynamic(this, &USessionLineWidget::JoinServer);
	return true;
}

void USessionLineWidget::SetVariables(int32 SessionIndexLocal, FString SessionName, int32 CurrentPlayers, int32 MaxPlayers)
{
	if (bVariablesSetted)return;
	SessionIndex = SessionIndexLocal;
	TextBlockSessionName->SetText(FText::FromString(SessionName));
	FString TempString = FString::FromInt(CurrentPlayers);
	TempString.Append("/");
	TempString.Append(FString::FromInt(MaxPlayers));
	TextBlockAmountOfPlayers->SetText(FText::FromString(TempString));
	bVariablesSetted = true;
}

void USessionLineWidget::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Session description widget - join button pressed"));
	UShooterGameInstance* GameInstance = Cast<UShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)GameInstance->JoinServer(SessionIndex);
}