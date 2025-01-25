#include "Tools/CRake.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Actor/Grow.h"

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
	FVector Start = OwnerCharacter->GetActorLocation();
	FRotator ActorRotation = OwnerCharacter->GetActorRotation();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ActorRotation);
	double Multiplier = 100.f;

	FVector MultipliedVector = ForwardVector * Multiplier;
	FVector End = Start + MultipliedVector;
	float SphereRadius = 100.f;

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(), End,End, SphereRadius, UEngineTypes::ConvertToTraceType(ECC_EngineTraceChannel1), false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.f);

	if (bHit)
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
		
	}

	if (bHit && HitResult.GetActor())
	{
		FVector HitActorLocatioin = HitResult.ImpactPoint;
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AGrow* SpawnGrow = GetWorld()->SpawnActor<AGrow>(AGrow::StaticClass(), HitActorLocatioin, SpawnRotation, SpawnParams);
		SpawnGrow->SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));

		UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
		UE_LOG(LogTemp, Log, TEXT("Collision Response to Soil: %d"), HitResult.GetComponent()->GetCollisionResponseToChannel(ECC_GameTraceChannel1));

	}

	

}

