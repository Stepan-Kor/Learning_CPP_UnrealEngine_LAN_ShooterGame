// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterWidget.h"
//#include "GameFrameWork/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	//WidgetClass=UShooter
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCapsuleSize(40,95,false);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	//Camera->AttachParent(GetCapsuleComponent());
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-40,0,60));
	Camera->bUsePawnControlRotation=true;
	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands Skeletal Mesh"));
	HandsMesh->SetupAttachment(Camera);
	HandsMesh->SetRelativeLocation(FVector(0,0,-160));
	WeaponMesh = CreateDefaultSubobject <USkeletalMeshComponent> (TEXT("Weapon Component"));
	//WeaponMesh->AttachToComponent(HandsMesh, FAttachmentTransformRules::KeepRelativeTransform,"GripPoint");
	WeaponMesh->SetupAttachment(HandsMesh, FName("GripPoint"));
	//WeaponMesh->Socket
	ProjectileSpawnParameters.SpawnCollisionHandlingOverride= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::MakeShot);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StopShot);
	PlayerInputComponent->BindAction("Pause",IE_Pressed,this,&APlayerCharacter::OpenPauseMenu);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp",this, &APlayerCharacter::LookUp);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(HandsMesh))HandsAnimationInstance = HandsMesh->GetAnimInstance();
	PlController = GetController <APlayerController>();

	if (IsValid(PlController)) {
		if (IsValid(WidgetClass))ScreenWidget = Cast <UShooterWidget>(CreateWidget(GetWorld(), WidgetClass, FName("Screen Widget")));
		if (IsValid(ScreenWidget)) {
			ScreenWidget->AddToViewport(0);
			if(PlController)	ScreenWidget->SetController(PlController);
			else UE_LOG(LogTemp, Warning, TEXT("Player %s have no controller pointer"), *this->GetName());
				}
		PlController->SetInputMode(FInputModeGameOnly());
		PlController->SetShowMouseCursor(false);
	}
}

void APlayerCharacter::MakeDamage(float Size)
{
	if (!bDead) {
		CurrentHealth -= Size;
		if (ScreenWidget)ScreenWidget->SetNewPercentage(CurrentHealth / 1000);
		//UE_LOG(LogTemp, Warning, TEXT("Player damaged %s"), *this->GetName());
		if (CurrentHealth < 0) { 
			bDead = true; 
			WarnWidgetAboutGameEnd(false);			
		}

	}
}

void APlayerCharacter::WarnWidgetAboutGameEnd(bool Success)
{
	UE_LOG(LogTemp,Warning,TEXT("Player character ends game"));
	if(IsValid(ScreenWidget))ScreenWidget->GameFinished(Success);
	if (IsValid(PlController)) { 
		bDead = true;
		PlController->SetInputMode(FInputModeUIOnly()); 
		PlController->SetShowMouseCursor(true);
		}
}

void APlayerCharacter::CameraBoolReset() {	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlayerCharacter::CameraBoolReset2);		}

void APlayerCharacter::CameraBoolReset2(){	bCameraWasUpdated = false;}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::MakeShot()
{	/*
	TArray <AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerCharacter::StaticClass(),Actors);
	APlayerCharacter* PlChar;
	int32 TempInt=HasAuthority()? 322 : 0;
	FColor Color = HasAuthority() ? FColor::Cyan : FColor::Orange;
	for (AActor* Actor : Actors) {
		FString TempString = Actor->GetName();
		bool bTemp;
		PlChar = Cast<APlayerCharacter>(Actor);
		if (PlChar) { 
			bTemp = IsValid(PlChar->GetController());
			if(bTemp)TempString.Append(PlChar->GetController()->HasAuthority() ? " Contr has autority." : " Contr dont has autority.");
			else TempString.Append(" Contr is not valid.");
			TempString.Append(PlChar->HasAuthority() ? " Char has autority." : " Char dont has autority.");
			switch (GetLocalRole()) {
			case ROLE_None: TempString.Append(" Role_none.");
				break;
			case ROLE_SimulatedProxy: TempString.Append(" Role_SimulatedProxy.");
				break;
			case ROLE_AutonomousProxy: TempString.Append(" ROLE_AutonomousProxy.");
				break;
			case ROLE_Authority: TempString.Append(" ROLE_Authority.");
				break;
			case ROLE_MAX: TempString.Append(" ROLE_MAX.");
				break;
			}
		}
		GEngine->AddOnScreenDebugMessage(TempInt, 522, Color, TempString);
		TempString = PlChar->GetName();
		TempString.Append( (IsLocallyControlled())? " localy conrolled" : " not localy controlled");
		GEngine->AddOnScreenDebugMessage(TempInt+52, 522, Color, TempString);
		TempInt++; 
	}*/
	if (HasAuthority()) Multi_UpdateCamera(Camera->GetRelativeRotation());
	else Server_UpdateCamera(Camera->GetRelativeRotation());
	
	if (HasAuthority()) { 		Multi_MakeShot();		UE_LOG(LogTemp, Warning, TEXT("%s have authority"),*GetName());	}
	else Server_MakeShot();
	//return;
	if (IsValid(ProjectileClass)) {
		FVector MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");
		AActor* TempActor = GetWorld()->SpawnActor <ACustomProjectile>(ProjectileClass, MuzzleLocation,
			WeaponMesh->GetSocketRotation("Muzzle"), ProjectileSpawnParameters);
		ACustomProjectile* Proj = Cast<ACustomProjectile>(TempActor);
		if (Proj) {
			Proj->bDamageDoneToSomething = false; 
			//Proj->SetOwner(Controller); 
			//if(IsLocallyControlled())			Proj->SetNetRole(ENetRole::ROLE_Authority);
		}
		if (IsValid(FireSound))			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, MuzzleLocation);
		if (IsValid(HandsAnimationInstance) && IsValid(FireMontage)) 		HandsAnimationInstance->Montage_Play(FireMontage, 1);
	}
}

void APlayerCharacter::Multi_MakeShot_Implementation()
{
	if (IsLocallyControlled())return;
	if (IsValid(ProjectileClass)) {
		FVector MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");
		AActor* TempActor = GetWorld()->SpawnActor <ACustomProjectile>(ProjectileClass, MuzzleLocation,
			WeaponMesh->GetSocketRotation("Muzzle"), ProjectileSpawnParameters);
		
		if (IsValid(FireSound))			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, MuzzleLocation);
		if (IsValid(HandsAnimationInstance) && IsValid(FireMontage)) 		HandsAnimationInstance->Montage_Play(FireMontage, 1);
	}
}

void APlayerCharacter::Server_MakeShot_Implementation(){	Multi_MakeShot();}

void APlayerCharacter::Multi_UpdateCamera_Implementation(FRotator NewRotation){	Camera->SetRelativeRotation(NewRotation);}

void APlayerCharacter::Server_UpdateCamera_Implementation(FRotator NewRotation)
{
	//GEngine->AddOnScreenDebugMessage(0,.1,FColor::Yellow,"Camera udated on server");
	Multi_UpdateCamera(NewRotation);
	Camera->SetRelativeRotation(NewRotation);
}

void APlayerCharacter::StopShot()	{		
	//UE_LOG(LogTemp, Warning, TEXT("Shot stoped"));		
}

void APlayerCharacter::MoveForward(float Axis){	if(Axis!=0 && bDead==false)	AddMovementInput(GetActorForwardVector(), Axis); }
void APlayerCharacter::MoveRight(float Axis){	if (Axis != 0 && bDead == false)		AddMovementInput(GetActorRightVector(), Axis); }
void APlayerCharacter::Turn(float Axis)	{		if (Axis != 0 && bDead == false) 	AddControllerYawInput(Axis * TurnRate * GetWorld()->GetDeltaSeconds()); }
void APlayerCharacter::LookUp(float Axis) {	if (Axis != 0 && bDead == false) {
		AddControllerPitchInput(Axis * LookUpRate * GetWorld()->GetDeltaSeconds());
		if (bCameraWasUpdated)	return;
		bCameraWasUpdated = true;
		if (HasAuthority()) { 
			Multi_UpdateCamera(Camera->GetRelativeRotation());
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlayerCharacter::CameraBoolReset2);
		}
		else {
			Server_UpdateCamera(Camera->GetRelativeRotation());
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlayerCharacter::CameraBoolReset);
		}
	}
}

void APlayerCharacter::OpenPauseMenu()
{
	if (ScreenWidget)ScreenWidget->OpenPauseMenu(true);
	else UE_LOG(LogTemp, Warning, TEXT("Player %s have no widget ponter"), *this->GetName());
}

void APlayerCharacter::OpenPauseMenuWithoutPause(bool NewVisibility)
{
	if (ScreenWidget && NewVisibility)ScreenWidget->OpenPauseMenu(false);
	if (ScreenWidget && !NewVisibility)ScreenWidget->HideMainMenu();
}

