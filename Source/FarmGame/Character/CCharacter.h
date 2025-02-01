#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ACRake;
class UUserWidget;
class AGrow;

UCLASS()
class FARMGAME_API ACCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Axis);
	void MoveRight(float Axis);

	void OnTurn(float Axis);

	void MouseLeft();
	

private:
	void SetCustomMouseCursor();

	void OpenSKillWindow();

	
	void PlaceFarm();

	void UpdateFarmPreview();


public:
	UFUNCTION(BlueprintCallable, Category = "Farm")
	void OnFarmIconClicked();
protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComp;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Tools")
	TSubclassOf<ACRake> RakeClass;

	UPROPERTY()
	TSubclassOf<UUserWidget> CursorClass;

	// 스킬 창 열기
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SkillWindowClass;

	UPROPERTY()
	UUserWidget* SkillWindowWidget;

	// 농작물 
	UPROPERTY(EditDefaultsOnly, Category = "Farm")
	TSubclassOf<AGrow> GrowClass;

	UPROPERTY()
	AGrow* PreviewFarmActor;

private:
	ACRake* Rake;

	bool plowing = true;

};
