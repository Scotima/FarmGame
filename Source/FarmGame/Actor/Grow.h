#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grow.generated.h"

class UStaticMeshComponent;

UCLASS()
class FARMGAME_API AGrow : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrow();

protected:
	virtual void BeginPlay() override;




public:	
	virtual void Tick(float DeltaTime) override;


private:
	void ChangeMeshState();


private:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;

	UPROPERTY()
	TArray<UStaticMesh*> MeshAssets;

private:
	int32 CurrentStateIndex;
	FTimerHandle ChangeMeshTimerHandle;
};
