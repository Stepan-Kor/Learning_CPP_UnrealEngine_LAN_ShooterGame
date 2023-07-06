// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Public/HealthInterface.h"
#include "CustomProjectile.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SHOOTERCPP_API APlayerCharacter : public ACharacter, public IHealthInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	UPROPERTY (VisibleDefaultsOnly)USkeletalMeshComponent* HandsMesh;
	UPROPERTY (VisibleDefaultsOnly) USkeletalMeshComponent* WeaponMesh;
	UPROPERTY (BlueprintReadWrite, EditDefaultsOnly)UCameraComponent* Camera;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Widgets") class UShooterWidget* ScreenWidget;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly) TSubclassOf<class UShooterWidget> WidgetClass;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FActorSpawnParameters ProjectileSpawnParameters;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MakeShot();
	void StopShot();
	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void Turn(float Axis);
	void LookUp (float Axis);
	void OpenPauseMenu();
	void OpenPauseMenuWithoutPause(bool NewVisibility);
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly) float TurnRate=55;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly) float LookUpRate=55;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly) TSubclassOf<ACustomProjectile> ProjectileClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly) USoundBase* FireSound;
	UPROPERTY(BlueprintReadOnly) UAnimInstance* HandsAnimationInstance;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly) UAnimMontage* FireMontage;
	UFUNCTION(NetMultiCast, Reliable) void Multi_MakeShot();
	void Multi_MakeShot_Implementation();
	UFUNCTION(Server, Reliable) void Server_MakeShot();
	void Server_MakeShot_Implementation();
	UFUNCTION(NetMultiCast, Reliable) void Multi_UpdateCamera(FRotator NewRotation);
	void Multi_UpdateCamera_Implementation(FRotator NewRotation);
	UFUNCTION(Server, Reliable) void Server_UpdateCamera(FRotator NewRotation);
	void Server_UpdateCamera_Implementation(FRotator NewRotation);
	APlayerController* PlController;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void MakeDamage(float Size) override;
	float CurrentHealth = 1000;
	bool bDead = false;
	void WarnWidgetAboutGameEnd(bool Success);
	bool bCameraWasUpdated=false;
	void CameraBoolReset();
	void CameraBoolReset2();
	//UFUNCTION() void TestFunction(IHealthInterface* Interface) {};

};
