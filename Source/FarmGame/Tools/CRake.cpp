#include "Tools/CRake.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

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
		RakeMesh->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
		RakeMesh->SetRelativeRotation(FRotator(0.f, -187.f, 0.f));
	}
}

void ACRake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACRake::Plowing()
{
	OwnerCharacter->PlayAnimMontage(WorkHoeMontage);
	SweepSingleByChannel();
	
}

void ACRake::SweepSingleByChannel()
{
	FVector Start = RakeMesh->GetComponentLocation();
	FVector ForwardVector = RakeMesh->GetComponentTransform().TransformVector(FVector::ForwardVector);
	FVector End = Start + ForwardVector * 100;
	float SphereRadius = 100.f;

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(), Start,End, SphereRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.f);

	if (bHit)
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
	}

}

