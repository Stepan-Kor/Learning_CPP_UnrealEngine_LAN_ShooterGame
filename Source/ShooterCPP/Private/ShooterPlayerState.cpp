
// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerState.h"
#include "ShooterCPP/PlayerCharacter.h"
#include "Kismet/Gameplaystatics.h"
#include "HealthInterface.h"

void AShooterPlayerState::BeginPlay()
{
	if (HasAuthority()) LocalPlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	else LocalPlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	GameInstance = GetGameInstance<UShooterGameInstance>();
}

void AShooterPlayerState::Server_MakeDamageToActor_Implementation(AActor* Target, float Size)
{
	if (IsValid(Target)) {
		IHealthInterface* HI = Cast <IHealthInterface>(Target);
		if (HI) { 
			HI->MakeDamage(Size);
			//UE_LOG(LogTemp, Warning, TEXT("Server Player state made damage to %s"),*Target->GetName());
		}
	}
}

void AShooterPlayerState::Multi_PauseAllClients_Implementation(bool NewPauseState)
{
	if (NewPauseState == false) {
		UE_LOG(LogTemp, Warning, TEXT("Player state pausing game %d ."), NewPauseState);
		if (LocalPlayerController)LocalPlayerController->SetPause(NewPauseState);
		//if (LocalPlayerController)LocalPlayerController->ServerPause();
		else UE_LOG(LogTemp, Warning, TEXT("Player state have not controller."));
	}
	APlayerCharacter* TempPlChar = Cast <APlayerCharacter>(LocalPlayerController->GetCharacter());
	UE_LOG(LogTemp, Warning, TEXT("Player state call open menu."));
	if (TempPlChar)TempPlChar->OpenPauseMenuWithoutPause(NewPauseState);
		
	

}

void AShooterPlayerState::Server_SetPauseSession_Implementation(bool NewState)
{
	Multi_PauseAllClients(NewState);
}

void AShooterPlayerState::PauseSession(bool NewState)
{
	if (LocalPlayerController) { 
		if (NewState) {
			Server_SetPauseSession(true);
			UE_LOG(LogTemp, Warning, TEXT("Player state called controller to pause."));
			if (HasAuthority()) {
				if (GameInstance && false) { if (!GameInstance->IsCurrentlyInSession()) { 
					LocalPlayerController->Pause();
					UE_LOG(LogTemp, Warning, TEXT("Player state localy pause game."));
					return;
					} 
				}
				//GetWorld()->GetFirstPlayerController()->ServerPause(); 
				TArray <AActor*> TempActors;
				bool bPaused = false;
				//APlayerController* TempPlContr;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), TempActors);
				for (AActor* Actor : TempActors) {
					if (Actor){APlayerController* TempPlContr = Cast<APlayerController>(Actor);
					if (TempPlContr) {
						if (!TempPlContr->IsLocalController()) {
							TempPlContr->ServerPause();
							bPaused = true;
							UE_LOG(LogTemp, Warning, TEXT("Player state called controller %s to pause online. %d"), *Actor->GetName(), TempPlContr->IsLocalController());
							break;
						}
					}
					}
				}

				if(!bPaused)LocalPlayerController->Pause();
			}
			else LocalPlayerController->ServerPause();
		}
		else {
			Server_SetPauseSession(false);
			UE_LOG(LogTemp, Warning, TEXT("Player state called controller to Unpause."));
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("Player state have not controller."));
}

