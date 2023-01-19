// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponPickup.generated.h"


/*
USTRUCT(BlueprintType)
struct FIKProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BluePrintReadWrite);
	class UAnimSequence* AnimPose;

	UPROPERTY(EditAnywhere, BluePrintReadWrite);
	FTransform CustomOffsetTransform;
};

*/


UCLASS()
class FIRST_PERSON_API AWeaponPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

/*
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Weapon");
		bool isObtained;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Weapon");
		int index;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components");
		class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components");
		class USkeletalMeshComponent* Mesh;
	

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Config")
		FIKProperties IKProperties;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Config")
		FTransform PlacementTransform;
	*/	
};
