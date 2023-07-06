// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ShooterGameInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessionsSearchDelegate, UShooterGameInstance*, GameInstance);

UCLASS()
class SHOOTERCPP_API UShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable) FSessionsSearchDelegate SessionsSearchDelegateList;
	UShooterGameInstance();
	void CreateSession(FString ServerName,FString HostName);
	void RefreshSessionsList();
	void JoinServer(int32 GameIndex);
	void LeaveServer();
	void RessetLeaveSessionBool();
	bool IsCurrentlyInSession();
	TSharedPtr <FOnlineSessionSearch> SessionSearch;
	bool bCurrentSessionExist = false;
protected:
	IOnlineSessionPtr SessionInterface;
	virtual void Init() override;
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);
	void OnRegisterPlayersComplete(FName InName, const TArray< FUniqueNetIdRef >& ID_Players, bool SomeBool);
	FName DefaultServerName = FName("TestShooterNetServer");
	bool bSearchInProgress=false;
	bool bJoiningInProgress = false;
	bool bSessionAlreadyDestroyed=false;
	bool bCreationOfSessionInProgress = false;
	FName CurrentSessionName;
	int32 LastSessionIndex;
};
