// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ShooterCPP/PlayerCharacter.h"
#include "ExitDoor.generated.h"

UCLASS()
class SHOOTERCPP_API AExitDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExitDoor();
	UPROPERTY(BlueprintReadOnly,VisibleDefaultsOnly) UBoxComponent* Box;
	UPROPERTY(BlueprintReadOnly,VisibleDefaultsOnly) USceneComponent* BaseScene;
	UPROPERTY(EditDefaultsOnly) UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION() void OnBoxOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
