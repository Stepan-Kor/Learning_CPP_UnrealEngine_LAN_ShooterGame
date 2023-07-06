// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemiesPlacer.h"

// Sets default values
AEnemiesPlacer::AEnemiesPlacer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* TempComp = CreateDefaultSubobject <USceneComponent>(TEXT ("Default Scene"));
	SetRootComponent(TempComp);
#if WITH_EDITOR
	RootComponent->bVisualizeComponent = true;
#endif
}

#if WITH_EDITOR
void AEnemiesPlacer::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	RootComponent->bVisualizeComponent = true;}
#endif

// Called when the game starts or when spawned
void AEnemiesPlacer::BeginPlay()
{
	if (!isOn) {
		Destroy();
		return;
	}
	Super::BeginPlay();
	UWorld* World= GetWorld();
	FActorSpawnParameters TempParameters;
	TempParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	int NumberForEnemy=0;
	while (true) { break; }
	bool bNegativeProgressionX = (SpawnPlaceDeltaX<0);
	if (SpawnPlaceDeltaX == 0)return;
	UE_LOG(LogTemp, Warning, TEXT("Start spawning enemies and cubes."));/*
	const FText TempString1 = FText::FromString("Cube");
	const FText TempString2 = FText::FromString("Enemy");*/
	const FString TempString1 = "Enemy";
	const FString TempString2 = "Cube";

	if (HasAuthority() && CubesClass && EnemiesClass) {
		for (float x = SpawnPlaceStartX; bNegativeProgressionX ? (x > SpawnPlaceEndX) : (x<SpawnPlaceEndX); x += SpawnPlaceDeltaX) {
			NumberForEnemy = FMath::RandRange (0, FMath::FloorToInt(DistanceMaxY*2/DistanceDeltaY));
			int i = 0;
			for (float y = -DistanceMaxY;	 y < DistanceMaxY ;		y+=DistanceDeltaY) {
				bool bIsEnemy = (i == NumberForEnemy);
				i++;
				UE_LOG(LogTemp, Warning, TEXT("Spawning %s in X: %f  Y: %f"),bIsEnemy ? *TempString1 : *TempString2,x,y);
				AActor* TempActor =World->SpawnActor<AActor>( bIsEnemy ? EnemiesClass : CubesClass,FVector(x,y,bIsEnemy ? HeightForEnemy : 350),
					FRotator(0,FMath::RandRange(0,360), 0),TempParameters);
				if (!bIsEnemy && TempActor) { 
					float RandFloat = FMath::RandRange(0.5f, 1.1f);
					TempActor->SetActorScale3D(FVector(RandFloat, RandFloat, RandFloat));
				
				}
			
			}

		}

	}
	Destroy();
}

// Called every frame
void AEnemiesPlacer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
