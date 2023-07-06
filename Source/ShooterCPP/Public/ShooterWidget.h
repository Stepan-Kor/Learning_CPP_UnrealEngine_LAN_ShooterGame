// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "ShooterWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERCPP_API UShooterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget)) UProgressBar* ProgressBar;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Restart;
	UPROPERTY(meta = (BindWidget)) UButton* Button_MainMenu;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Exit;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Continue;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_EndType;
	UPROPERTY(meta = (BindWidget)) UVerticalBox* VerticalBox_Menus;
	class APlayerController* PlController;
	//UPROPERTY()
public:
	void SetNewPercentage(float Amount);
	void GameFinished(bool Success);
	void OpenPauseMenu(bool WithPause);
	UFUNCTION()void Continue();
	UFUNCTION()void BackToMainMenu();
	UFUNCTION()void HideMainMenu();
	virtual bool Initialize() override;
	UFUNCTION()void ExitGame();
	UFUNCTION()void RestartGame();
	void SetController(APlayerController* NewController);
	class AShooterPlayerState* PlState;
	class UShooterGameInstance* GameInstance;
};
