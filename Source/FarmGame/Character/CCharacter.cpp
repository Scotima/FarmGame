#include "Character/CCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"	
#include "GameFramework/CharacterMovementComponent.h"

ACCharacter::ACCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	

	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAsset(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	ConstructorHelpers::FClassFinder<UAnimationAsset> AnimInstanceClass(TEXT("/Game/Characters/Mannequins/Animations/ABP_Manny"));
	if (AnimInstanceClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass.Class);
	}



	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 100));
	SpringArmComp->SetRelativeRotation(FRotator(-88, 0, 0));
	SpringArmComp->TargetArmLength = 800.f;
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->SocketOffset = FVector(0, 60, 0);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp -> SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;

}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 플레이 했을때 마우스 커서 보이게 하기.
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		//PlayerController->bEnableMouseOverEvents = true;
	}


	
}

void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ACCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ACCharacter::OnTurn);
	
}

void ACCharacter::MoveForward(float Axis)
{
	FRotator ControlRot = FRotator(0, GetControlRotation().Yaw, 0);
	FVector Direction = FQuat(ControlRot).GetForwardVector().GetSafeNormal2D();

	AddMovementInput(Direction, Axis);
}

void ACCharacter::MoveRight(float Axis)
{
	FRotator ControlRot = FRotator(0, GetControlRotation().Yaw, 0);
	FVector Direction = FQuat(ControlRot).GetRightVector().GetSafeNormal2D();

	AddMovementInput(Direction, Axis);
}

void ACCharacter::OnTurn(float Axis)
{
	AddControllerYawInput(Axis);
}






