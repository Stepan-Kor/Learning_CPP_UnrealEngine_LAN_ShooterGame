// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor.h"

// Sets default values
AExitDoor::AExitDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	BaseScene = CreateDefaultSubobject<USceneComponent>("Root scene component");
	SetRootComponent(BaseScene);
	PrimaryActorTick.bCanEverTick = true;
	Box = CreateDefaultSubobject<UBoxComponent>("Box Collision Component",false);
	Box->SetGenerateOverlapEvents(true);
	Box->SetBoxExtent(FVector(10,100,200),false);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetupAttachment(RootComponent);
	Box->SetRelativeLocation(FVector(0,0,200));
	Box->SetHiddenInGame(false);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorWay",false);
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AExitDoor::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(Box)) {
		Box->OnComponentBeginOverlap.AddDynamic(this, &AExitDoor::OnBoxOverlaped);
		//UE_LOG(LogTemp, Warning, TEXT( "Door have pointer to box"));
		//Box->OnComponentBeginOverlap.AddDynamic(this,&AExitDoor::OnBoxOverlaped);
		//bool bTempBool = Box->OnComponentBeginOverlap.IsBound();
		//UE_LOG(LogTemp, Warning, TEXT("Door box bounded %d"),bTempBool);
		//FString TempString;
		//TempString.Append(Box->GetGenerateOverlapEvents() ? "can generate overlaps  "  :  "can NOT generate overlaps  ");
		//Box->SetCollisionResponseToChannel(ECC_Destructible,ECR_Overlap);
		//TempString.Append(Box->GetCollisionResponseToChannel(ECC_Destructible) ? " destr - ignored "  :  " destr - overlaped " );
		//TempString.Append(Box->GetCollisionResponseToChannel(ECC_Destructible) ? " destr - ignored "  :  " destr - overlaped " );
		//int32 TempInt = Box->GetCollisionResponseToChannel(ECC_Destructible);
		//TempString.Append(FString::FromInt(TempInt ));
		//Box->GetCollisionEnabled();
		//GEngine->AddOnScreenDebugMessage(0,145,FColor::Orange,TempString);

	}
	
}

void AExitDoor::OnBoxOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (!IsValid(OtherActor))return;
	APlayerCharacter* PlChar = Cast <APlayerCharacter>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("Door have been overlaped"));
	if (IsValid(PlChar)) { 
		PlChar->WarnWidgetAboutGameEnd(true);
		//GEngine->AddOnScreenDebugMessage(0, 5, FColor::Orange, "Door overlaped PLAYER"); 
	}
}

// Called every frame
void AExitDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

