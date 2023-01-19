// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"

#include "Animation/SkeletalMeshActor.h"
#include "BaseWeapon.h"

#include "MyCharacter.generated.h"

UCLASS()
class FIRST_PERSON_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void MoveForward(float value);
	void MoveRight(float value);

	void CheckJump();
	void Sprint();
	void OnFire();
	void Melee();

	void Dash();
	void ResetDash();
	void Interact();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SwitchToNextWeapon();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void SwitchWeaponMesh(int _index);



	UPROPERTY()
		bool jumping;
	UPROPERTY()
		int jumpCount;
	UPROPERTY()
		float speed;
	UPROPERTY()
		bool walking;
	UPROPERTY()
		bool hasDashed;

	


	
	UPROPERTY(VisibleDefaultsOnly, BluePrintReadWrite, Category = Mesh, DisplayName = "HandsMesh")
		class USkeletalMeshComponent* HandsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class USkeletalMeshComponent* GunMesh;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere, Category = Mesh)
		class USceneComponent* MuzzleLocation;

	UPROPERTY(EditAnywhere, Category = GamePlay)
		FVector GunOffset;

	

	

	

	
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;
	

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
		class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
		class UAnimMontage* FireAnimation;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
		class UAnimSequence* MeleeAnimation;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
		class UAnimMontage* MeleeAnimation;



	// The Array of weapon class 
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	//	 TArray<TSubclassOf<class AWeaponPickup>> weapons;

	// The index of the weapon the character is currently using
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapons)
		int weaponIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapons)
		TArray<ABaseWeapon*> weapons;




	class UAnimInstance* AnimInstance;
	class UWorld* World;

	FRotator SpawnRotation;
	FVector SpawnLocation;
	
};
