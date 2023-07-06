// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemiesPlacer.generated.h"

UCLASS()
class SHOOTERCPP_API AEnemiesPlacer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemiesPlacer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere) TSubclassOf <AActor> CubesClass;
	UPROPERTY(EditAnywhere) TSubclassOf <AActor> EnemiesClass;
	UPROPERTY(EditAnywhere) float SpawnPlaceStartX=1500;
	UPROPERTY(EditAnywhere) float SpawnPlaceEndX=4500;
	UPROPERTY(EditAnywhere) float SpawnPlaceDeltaX=750;
	UPROPERTY(EditAnywhere) float DistanceMaxY=700;
	UPROPERTY(EditAnywhere) float DistanceDeltaY=400;
	UPROPERTY(EditAnywhere) float HeightForEnemy=98;
	UPROPERTY(EditAnywhere) bool isOn=true;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
