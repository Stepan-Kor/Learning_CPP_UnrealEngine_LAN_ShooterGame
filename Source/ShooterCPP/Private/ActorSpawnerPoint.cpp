// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorSpawnerPoint.h"
//#include ""

// Sets default values
AActorSpawnerPoint::AActorSpawnerPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* TempSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Component"));
	//if(TempSceneComponent)TempSceneComponent->bVisualizeComponent = true;
	SetRootComponent(TempSceneComponent);
#if WITH_EDITOR
	RootComponent->bVisualizeComponent = true;
#endif
	
}

#if WITH_EDITOR
void AActorSpawnerPoint::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent){
	RootComponent->bVisualizeComponent = true;
}
#endif

// Called when the game starts or when spawned
void AActorSpawnerPoint::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority()) return;
	FVector TempVector = AdditionalVector + GetActorLocation();
	FActorSpawnParameters SpawnInfo;
	FRotator TempRotator = FRotator(0,GetActorRotation().Yaw, 0);
	AActor* TempActor =GetWorld()->SpawnActor<AActor>(ClassOfActorToSpawn,TempVector,TempRotator,SpawnInfo);
	if (TempActor)TempActor->OnDestroyed.AddDynamic(this,&AActorSpawnerPoint::ActorDestroyed);
	//this->Destroy();
}

// Called every frame
void AActorSpawnerPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorSpawnerPoint::ActorDestroyed(AActor* DestroyedActor)
{
	FVector TempVector = AdditionalVector + GetActorLocation();
	FActorSpawnParameters SpawnInfo;
	AActor* TempActor = GetWorld()->SpawnActor<AActor>(ClassOfActorToSpawn, TempVector, FRotator(0, GetActorRotation().Yaw, 0) , SpawnInfo);
	if (TempActor)TempActor->OnDestroyed.AddDynamic(this, &AActorSpawnerPoint::ActorDestroyed);

}

