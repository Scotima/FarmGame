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
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
};
