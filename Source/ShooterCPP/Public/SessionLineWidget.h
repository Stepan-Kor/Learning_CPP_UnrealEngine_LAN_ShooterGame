// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "SessionLineWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERCPP_API USessionLineWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget)) UButton* ButtonJoin;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlockSessionName;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlockAmountOfPlayers;
	int32 SessionIndex = 0;
	UFUNCTION()void JoinServer();

	virtual bool Initialize() override;
	void SetVariables(int32 SessionIndexLocal,FString SessionName, int32 CurrentPlayers, int32 MaxPlayers);
protected:
	bool bVariablesSetted = false;

};
