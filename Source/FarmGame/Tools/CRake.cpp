#include "Tools/CRake.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"

ACRake::ACRake()
{
	PrimaryActorTick.bCanEverTick = true;

	RakeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RakeMesh"));
	RootComponent = RakeMesh;

	ConstructorHelpers::FObjectFinder<UStaticMesh> RakeMeshAsset(TEXT("/Game/3D_LOW_POLY_FarmerPack/Props/SM_Rake"));

	if (RakeMeshAsset.Succeeded())
	{
		RakeMesh->SetStaticMesh(RakeMeshAsset.Object);
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> WorkHoeMontageAsset(TEXT("/Game/AnimationRetarget/anim_Farmer_work_Hoe_Montage"));

	if (WorkHoeMontageAsset.Succeeded())
	{
		WorkHoeMontage = WorkHoeMontageAsset.Object;
	}

	HandSocket = "middle_02_rsocket";

}

void ACRake::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HandSocket);

	if (RakeMesh)
	{
		// 콜리전 비활성화
		RakeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 상대 위치 설정
		RakeMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	}
}

void ACRake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

