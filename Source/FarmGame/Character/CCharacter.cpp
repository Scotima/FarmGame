#include "Character/CCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"	
#include "GameFramework/CharacterMovementComponent.h"
#include "Tools/CRake.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Actor/Grow.h"

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

	ConstructorHelpers::FClassFinder<ACRake> ToolClass(TEXT("/Game/FarmTools/BP_CRake"));

	if (ToolClass.Succeeded())
	{
		RakeClass = ToolClass.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> CursorAsset(TEXT("/Game/UI/WG_MouseCursor"));
	if (CursorAsset.Succeeded())
	{
		CursorClass = CursorAsset.Class;
	}

	//스킬창 위젯 로드
	static ConstructorHelpers::FClassFinder<UUserWidget> SkillWindowAsset(TEXT("/Game/UI/WG_SkillWindow"));
	if (SkillWindowAsset.Succeeded())
	{
		SkillWindowClass = SkillWindowAsset.Class;
	}

	// 농작물 로드
	static ConstructorHelpers::FClassFinder<AGrow> GrowAsset(TEXT("/Game/FarmCrops/BP_Crops"));

	if (GrowAsset.Succeeded())
	{
		GrowClass = GrowAsset.Class;
	}


	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 100));
	SpringArmComp->SetRelativeRotation(FRotator(-88, 0, 0));
	SpringArmComp->TargetArmLength = 800.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SocketOffset = FVector(0, 60, 700);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp -> SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 플레이 했을때 마우스 커서 보이게 하기.
	SetCustomMouseCursor();

	if (RakeClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		Rake = GetWorld()->SpawnActor<ACRake>(RakeClass, SpawnParams);

	}

	if (SkillWindowClass)
	{
		SkillWindowWidget = CreateWidget<UUserWidget>(GetWorld(), SkillWindowClass);
	}


	
}

void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateFarmPreview();
	
}

void ACCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ACCharacter::OnTurn);
	PlayerInputComponent->BindAction("MouseLeft", IE_Pressed, this, &ACCharacter::MouseLeft);

	PlayerInputComponent->BindAction("SkillWindow", IE_Pressed, this, &ACCharacter::OpenSKillWindow);
	
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

void ACCharacter::MouseLeft()
{
	if (Rake && plowing)
	{
		Rake->Plowing();
	}
	PlaceFarm();
	
}

void ACCharacter::SetCustomMouseCursor()
{
	// 플레이 했을때 마우스 커서 보이게 하기.
	APlayerController* PlayerController = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is nullptr"));
		return;
	}

	if (CursorClass)
	{
		UUserWidget* CursorWidget = CreateWidget<UUserWidget>(GetWorld(), CursorClass);
		if (CursorWidget)
		{
			PlayerController->SetMouseCursorWidget(EMouseCursor::Default, CursorWidget);
			PlayerController->bShowMouseCursor = true;

			UE_LOG(LogTemp, Log, TEXT("Custom mouse cursor set."));
		}

	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Custom Cursor WidgetAsset is null"));
	}
	
}

void ACCharacter::OpenSKillWindow()
{
	if (SkillWindowWidget)
	{
		bool IsVisible = SkillWindowWidget->IsInViewport();
		if (IsVisible)
		{
			SkillWindowWidget->RemoveFromViewport();
			plowing = true;
		}

		else
		{
			SkillWindowWidget->AddToViewport();
			plowing = false;
		}
	}
}

void ACCharacter::OnFarmIconClicked()
{
	if (!PreviewFarmActor && GrowClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		PreviewFarmActor = GetWorld()->SpawnActor<AGrow>(GrowClass, SpawnParams);

		if (PreviewFarmActor)
		{
			PreviewFarmActor->SetActorEnableCollision(false);
		}
	}
}

void ACCharacter::PlaceFarm()
{
	if (PreviewFarmActor)
	{
		FTransform FarmTransform = PreviewFarmActor->GetActorTransform();
		AGrow* PlacedFarm = GetWorld()->SpawnActor<AGrow>(GrowClass, FarmTransform);

		if (PlacedFarm)
		{
			UE_LOG(LogTemp, Log, TEXT("FarmPlaced"));
		}

		PreviewFarmActor->Destroy();
		PreviewFarmActor = nullptr;
	}
}

void ACCharacter::UpdateFarmPreview()
{

	if (PreviewFarmActor)
	{
		FVector WorldLocation, WorldDirection;
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		if (PlayerController && PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			FVector TargetLocation = WorldLocation + WorldDirection * 1000.f;
			TargetLocation.Z = 0;
			PreviewFarmActor->SetActorLocation(TargetLocation);

		}
	}
}






