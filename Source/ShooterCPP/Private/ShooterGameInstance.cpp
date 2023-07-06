// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameInstance.h"
#include "Kismet/GameplayStatics.h"

UShooterGameInstance::UShooterGameInstance()
{
	
}


void UShooterGameInstance::Init()
{
	Super::Init();
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get()) {
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,&UShooterGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,&UShooterGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this,&UShooterGameInstance::OnJoinSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this,&UShooterGameInstance::OnDestroySessionComplete);
			SessionInterface->OnRegisterPlayersCompleteDelegates.AddUObject(this, &UShooterGameInstance::OnRegisterPlayersComplete);
		}
	}
}

void UShooterGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	bCreationOfSessionInProgress = false;
	bCurrentSessionExist = bWasSuccessful;
	if (bWasSuccessful) {
		CurrentSessionName = SessionName;
		GetWorld()->ServerTravel("/Game/FirstPersonCPP/Maps/FirstPersonExampleMap?Listen");
	}
	else {

		CurrentSessionName = "";
		UE_LOG(LogTemp, Warning, TEXT("Didnt create session . . . "));
		if (SessionInterface->GetSessionState(FName(SessionName)) == EOnlineSessionState::InProgress) {
			GetWorld()->ServerTravel("/Game/FirstPersonCPP/Maps/FirstPersonExampleMap?Listen");
			/*
			FOnlineSessionSettings* TempSettings = SessionInterface->GetSessionSettings(FName(*SessionName));
			TempSettings->resu;
			SessionInterface->JoinSession(0, SessionName, );
			*/
		}
		else		{
			SessionInterface->DestroySession(SessionName);
			CreateSession(SessionName.ToString(), "DefaultHostName");
			UE_LOG(LogTemp, Warning, TEXT("session state wrong"));
		}
	}
}

void UShooterGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	bCurrentSessionExist = false;
	CurrentSessionName = "";
}

void UShooterGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	bSearchInProgress = false;
	SessionsSearchDelegateList.Broadcast(this);
	//SessionSearch->SearchResults;
}

void UShooterGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	bJoiningInProgress = false;
	UE_LOG(LogTemp, Warning, TEXT("GameInstance join to session complete %d"), JoinResult);

	if (JoinResult == EOnJoinSessionCompleteResult::Success || JoinResult == EOnJoinSessionCompleteResult::AlreadyInSession) {
		APlayerController* PlController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		bCurrentSessionExist = true;
		CurrentSessionName = SessionName;
		if (PlController) {
			FString URLString = "";
			SessionInterface->GetResolvedConnectString(SessionName, URLString);
			if (URLString != "") {
				UE_LOG(LogTemp, Warning, TEXT("Game Instance: client traveling by url %s"),*URLString);
				bSessionAlreadyDestroyed = false;
				PlController->ClientTravel(URLString, ETravelType::TRAVEL_Absolute);
			}
			else			UE_LOG(LogTemp, Warning, TEXT("URL String empty"));
		}
	}

		//FString TempString = UEnum::GetValueAsString<EOnJoinSessionCompleteResult::Type>(JoinResult);
		//UE_LOG(LogTemp, Warning, TEXT("Bad join result %s"), *TempString);
		//EOnJoinSessionCompleteResult::
	else { 
		if (bSessionAlreadyDestroyed) {
			FTimerHandle TempHandle;
			GetTimerManager().SetTimer(TempHandle,this,&UShooterGameInstance::RessetLeaveSessionBool, 0.1f, false);
		}
		else { 
			bSessionAlreadyDestroyed = true;
			LeaveServer();
			JoinServer(LastSessionIndex);
		}
		UE_LOG(LogTemp, Warning, TEXT("Bad join result %d"), JoinResult); 
	}

	
}

void UShooterGameInstance::OnRegisterPlayersComplete(FName InName, const TArray< FUniqueNetIdRef > & ID_Players, bool SomeBool)
{
	bCurrentSessionExist = ID_Players.Num()==0;
}


void UShooterGameInstance::CreateSession(FString SessionName, FString HostName)
{
		if (bCreationOfSessionInProgress)return;
		UE_LOG(LogTemp, Warning, TEXT("GameInstance creating session . . . "));
		FOnlineSessionSettings SSettings;
		SSettings.bAllowJoinInProgress = true;
		SSettings.bIsDedicated = false;
		//SSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
		SSettings.bIsLANMatch = true;
		SSettings.bShouldAdvertise = true;
		SSettings.bUsesPresence = true;
		SSettings.NumPublicConnections = 1;
		SSettings.NumPrivateConnections = 0;
		SSettings.Set(FName("SERVER_NAME_KEY"),SessionName,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		FName TempName = (HostName == "") ? DefaultServerName : FName(*HostName);
		SessionInterface->CreateSession(0, TempName, SSettings);
		bCreationOfSessionInProgress = true;
}

void UShooterGameInstance::RefreshSessionsList()
{
	if (!bSearchInProgress) {
		bSearchInProgress = true;
		SessionSearch = MakeShareable(new FOnlineSessionSearch());
		SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
		SessionSearch->MaxSearchResults = 10000;
		SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("Game Instance searching for sessions . . ."));		
	}
	else UE_LOG(LogTemp, Warning, TEXT("Game Instance search allready in progress . . ."));
}

void UShooterGameInstance::JoinServer(int32 GameIndex)
{
	if (bJoiningInProgress == true)	return;
	if (SessionSearch->SearchResults.Num() > GameIndex) {
		bJoiningInProgress = true;
		LastSessionIndex = GameIndex;
		SessionInterface->JoinSession(0, DefaultServerName, SessionSearch->SearchResults[GameIndex]);
	}
}

void UShooterGameInstance::LeaveServer()
{
	bCurrentSessionExist = false;
	SessionInterface->DestroySession(CurrentSessionName);
}

void UShooterGameInstance::RessetLeaveSessionBool()
{
	bSessionAlreadyDestroyed = false;
}

bool UShooterGameInstance::IsCurrentlyInSession()
{
	if (bCurrentSessionExist) {
		FOnlineSessionSettings*  TempSettings = SessionInterface->GetSessionSettings(CurrentSessionName);
		if (TempSettings->NumPrivateConnections > 0) return true;
		else return false;
	
	}
	TArray <AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerController::StaticClass(),TempActors);
	bool bHaveConections=false;
	for (AActor* Actor : TempActors) {
		if (Actor) {
			if (APlayerController* TempController = Cast<APlayerController>(Actor)) {
				if (!TempController->IsLocalController()) {
					bHaveConections = true;
					break;
				}

			}

		}

	}
	if (bHaveConections) return true;
	else 	return false;
}
