// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
ACharacter_Controller::ACharacter_Controller()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	// ī�޶� ��ġ�� ���� =>  #include "GameFramework/SpringArmComponent.h" ���� �ؾ� ��
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; // ��ġ�� ����
	CameraBoom->bUsePawnControlRotation = true;

	// ī�޶� ���� => #include "Camera/CameraComponent.h" ���� �ؾ� ��
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	
	FollowCamera->bUsePawnControlRotation = false;

	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate= FRotator(0.f,740.f,0.f); // RotationRate = ENGINE_API FFindFloorResult ����ü�� �ִ� FRotator ����
	GetCharacterMovement()->JumpZVelocity = 650.f; // ���� ���� ����
	GetCharacterMovement()->AirControl = 0.2f; // ���߿��� �̵��� ����


	/* ���콺 �̵��� ���� ĳ���Ͱ� ���ٸ�? ����ϱ�
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;*/
}

// Called when the game starts or when spawned
void ACharacter_Controller::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacter_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacter_Controller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacter_Controller::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacter_Controller::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacter_Controller::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacter_Controller::LookUpAtRate);


	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void ACharacter_Controller::MoveRight(float Value)
{
	if (Controller != nullptr && (Value != 0.0f))
	{
		//Find out wich way is forward
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}


void ACharacter_Controller::MoveForward(float Value)
{
	if (Controller != nullptr && (Value != 0.0f))
	{
		// Find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACharacter_Controller::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacter_Controller::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate* GetWorld()->GetDeltaSeconds());
}

