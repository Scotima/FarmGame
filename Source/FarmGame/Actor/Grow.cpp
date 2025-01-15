

#include "Actor/Grow.h"

AGrow::AGrow()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/Environment/Meshes/Crops/SM_Crop_Wheat_01"));

	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
}

void AGrow::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

