// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SessionLineWidget.h"
#include "ShooterGameInstance.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERCPP_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget)) UButton* ButtonExit;
	UPROPERTY(meta = (BindWidget)) UButton* ButtonRefreshList;
	UPROPERTY(meta = (BindWidget)) UButton* ButtonCreateServer;
	UPROPERTY(meta = (BindWidget)) UEditableTextBox* EditableTextBoxServerName;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlockRefreshList;
	UPROPERTY(meta = (BindWidget)) UScrollBox* ScrollBoxList;
	virtual bool Initialize() override;
	UFUNCTION()void Exit();
	UFUNCTION()void CreateServer();
	UFUNCTION()void RefreshList();
	UFUNCTION()void SessionsListUpdated(UShooterGameInstance* InstanceOfGame);
	UShooterGameInstance* GameInstance;
	APlayerController* PlayerController;
	UPROPERTY(EditAnywhere)TSubclassOf <USessionLineWidget> WidgetLineClass;
protected:
	TArray <FOnlineSessionSearchResult> SearchResults;	
	FString DefaultSessionName="Default Session Name";
	FString DefaultHostName= "HostName123";
};
