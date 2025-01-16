#include "Actor/Grow.h"

AGrow::AGrow()
{
	PrimaryActorTick.bCanEverTick = true;


	//�迭 ������ �ʱ�ȭ.
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	RootComponent = MeshComp;
	
	// ���¿������� ã�� �ϰ� MeshAssets�� ����.
	
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset0(TEXT("/Game/Environment/Meshes/Crops/SM_Crop_Wheat_01"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset1(TEXT("/Game/Environment/Meshes/Crops/SM_Crop_Wheat_02"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset2(TEXT("/Game/Environment/Meshes/Crops/SM_Crop_Wheat_03"));
		
	

	if (MeshAsset0.Succeeded()&& MeshAsset1.Succeeded()&& MeshAsset2.Succeeded())
	{
		MeshAssets.Add(MeshAsset0.Object);
		MeshAssets.Add(MeshAsset1.Object);
		MeshAssets.Add(MeshAsset2.Object);

		MeshComp->SetStaticMesh(MeshAssets[0]);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Mesh Asset"));
	}
	

	CurrentStateIndex = 0;
	
}

void AGrow::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ChangeMeshTimerHandle, this, &AGrow::ChangeMeshState, 2.0f, true);
	
}

void AGrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrow::ChangeMeshState()
{
	if (MeshAssets.Num() == 0 || !MeshAssets.IsValidIndex(CurrentStateIndex))
	{
		GetWorldTimerManager().ClearTimer(ChangeMeshTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("MeshAssets is empty or CurrentStateIndex is out of range. Timer Stopped."));
	}

	
	
	MeshComp->SetStaticMesh(MeshAssets[CurrentStateIndex]);

	

	// ���¸� �������� ������Ʈ
	CurrentStateIndex++;

	// ������ ���¿� �����ϸ� Ÿ�̸� ����
	if (CurrentStateIndex >= MeshAssets.Num())
	{
		GetWorldTimerManager().ClearTimer(ChangeMeshTimerHandle);
		UE_LOG(LogTemp, Log, TEXT("Reached the last mesh state. Timer stopped."));
	}
}

