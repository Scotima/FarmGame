
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRake.generated.h"

class UStaticMeshComponent;
class UAnimMontage;
class ACharacter;

UCLASS()
class FARMGAME_API ACRake : public AActor
{
	GENERATED_BODY()
	
public:	
	ACRake();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	void Plowing();
	void SweepSingleByChannel();

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* RakeMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Montages");
	UAnimMontage* WorkHoeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Socket");
	FName HandSocket;

private:
	ACharacter* OwnerCharacter;

};
