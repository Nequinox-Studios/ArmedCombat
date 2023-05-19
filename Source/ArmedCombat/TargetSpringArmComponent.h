#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"

#include "TargetSpringArmComponent.generated.h"

UCLASS(ClassGroup=Camera, meta=(BlueprintSpawnableComponent), hideCategories=(Mobility))
class UTargetSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock Camera")
	float MaxTargetLockDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock Camera")
	bool bDrawDebug;


public:
	UTargetSpringArmComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleLockOn(AActor** Target);
	void EndTargetLockOn();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Target Lock Camera")
	bool IsCameraLockedToTarget();

private:
	AActor* CameraTarget {nullptr};
};