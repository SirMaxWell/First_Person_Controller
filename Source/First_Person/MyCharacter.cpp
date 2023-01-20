
// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"


#include "Projectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "First_PersonGameModeBase.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 95.0f);

	// Camera Setup
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->AddRelativeLocation(FVector(-40, 1.75, 64));
	FirstPersonCamera->bUsePawnControlRotation = true;

	// Creates Mesh for the fps hands model
	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));


	// Hand Model Setup
	HandsMesh->SetOnlyOwnerSee(true);
	HandsMesh->SetupAttachment(FirstPersonCamera);
	HandsMesh->bCastDynamicShadow = false;
	HandsMesh->CastShadow = false;
	HandsMesh->AddRelativeRotation(FRotator(2.0f, -20.0f, 5.0f));
	HandsMesh->AddRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunMesh->SetOnlyOwnerSee(true);
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

	MuzzleLocation = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(GunMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.0f, -10.0f));

	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Setting values to default 
	weaponIndex = 0; 
	jumping = false; 
	jumpCount = 0;
	speed = 0.5f;
	walking = true;
	
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	// attaches the gun to grip point, its located on the hand mesh 
	GunMesh->AttachToComponent(HandsMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
	World = GetWorld();
	AnimInstance = HandsMesh->GetAnimInstance();
	
	
	
}

void AMyCharacter::MoveForward(float value)
{
	if(value != 0.0f)
	AddMovementInput(GetActorForwardVector() * value * speed);
}

void AMyCharacter::MoveRight(float value)
{
	if (value != 0.0f)
	AddMovementInput(GetActorRightVector() * value * speed);
}



void AMyCharacter::CheckJump()
{
	// starts off false then flips each time the player jumps 
	// allows the player to hold jump to continue jumping 
	if(jumping)
	{
		jumping = false;
	}
	else
	{
		jumping = true;
		jumpCount++;
		if (jumpCount == 2)
		{
			LaunchCharacter(FVector(0, 0, 500), false, true);
		}
	}
}

void AMyCharacter::Sprint()
{
	walking = !walking; // if sprinting then we aren't walking 
	if (walking)
	{
		speed = 0.5f;
	}
	else
	{
		speed = 1;
	}

}

void AMyCharacter::OnFire()
{
	if (World != NULL)
	{
		SpawnRotation = GetControlRotation();
		if (MuzzleLocation != nullptr)
		{
			SpawnLocation = MuzzleLocation->GetComponentLocation();
		}
		else
		{
			SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);
		}

		// Will not spawn if it overlapping with something when colliding // Will Spawn near by 
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawns the projectile when firing
		World->SpawnActor<AProjectile>(Projectile, SpawnLocation, SpawnRotation, ActorSpawnParams);


		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
		if (FireAnimation != NULL  && AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.0f);
			UE_LOG(LogTemp, Error, TEXT("Thing"));
			
		}

	}
}

void AMyCharacter::Melee()
{
	if (MeleeAnimation != NULL && AnimInstance != NULL)
	{
		AnimInstance->Montage_Play(MeleeAnimation, 1.0f);
		UE_LOG(LogTemp, Error, TEXT("Melee"));
		
	}
}


void AMyCharacter::Dash()
{
	FCollisionResponseParams ResponseParams;
	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	QueryParams.AddIgnoredComponent(GetMesh()); 
	FHitResult hit;
	const FVector StartTrace = GetMesh()->GetSocketLocation(FName("DashSocket")); // Dash Docket Located on Player Arm Mesh 
	FRotator CurrentRotation = GetMesh()->GetSocketRotation(FName("DashSocket"));
	FVector EndTrace = StartTrace + CurrentRotation.Vector() * 1000.0f;
	GetWorld()->LineTraceSingleByChannel(hit, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, QueryParams, ResponseParams);
	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), false, -1, 0, 1.333); // Draws a debug line to Show how far you can go 
	if (!hasDashed)
	{
		SetActorLocation(EndTrace, true);
		hasDashed = true;
		FTimerHandle DashHandle;
		GetWorldTimerManager().SetTimer(DashHandle, this, &AMyCharacter::ResetDash, 1.0f, false);

	}
}

void AMyCharacter::ResetDash()
{
	hasDashed = false;
}

void AMyCharacter::SwitchToNextWeapon()
{
	bool success = false;
	for (int i = 0; i < weapons.Num(); i++)
	{
		if (i > weaponIndex)
		{
			if (weapons[i]->isObtained)
			{
				success = true;
				weaponIndex = i;
				SwitchWeaponMesh(weapons[i]->index);
				break;
			}
		}
	}

	if (!success)
	{
		weaponIndex = 0;
		SwitchWeaponMesh(weaponIndex);
	}
}

void AMyCharacter::Interact()
{
	// Done through BP
}







// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (jumping)
	{
		Jump();
	}

}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	jumpCount = 0;
}


// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMyCharacter::CheckJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMyCharacter::CheckJump);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMyCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMyCharacter::Sprint);

	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AMyCharacter::Dash);

	PlayerInputComponent->BindAction(TEXT("Melee"), IE_Pressed, this, &AMyCharacter::Melee);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AMyCharacter::OnFire);

	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AMyCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Switch"), IE_Pressed, this, &AMyCharacter::SwitchToNextWeapon);
}


