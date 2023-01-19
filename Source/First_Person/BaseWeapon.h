// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class FIRST_PERSON_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Weapon");
	bool isObtained;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Weapon");
	int index;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Weapon");
	FString name;
};