#include "TargetSpringArmComponent.h"

UTargetSpringArmComponent::UTargetSpringArmComponent()
{
	MaxTargetLockDistance = 900.f;
	bDrawDebug = true;
}


void UTargetSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsCameraLockedToTarget())
	{
		if ((CameraTarget->GetActorLocation() - GetOwner()->GetActorLocation()).Size() > MaxTargetLockDistance)
		{
			EndTargetLockOn();
		}
	}

	if (bDrawDebug)
	{
		// Max Target Lock Distance
		DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), MaxTargetLockDistance, 32, FColor::Cyan);

		// Focus point 
		DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation() + TargetOffset, 3, 4, FColor::Red);
	}
}

void UTargetSpringArmComponent::ToggleLockOn(AActor** Target)
{

	if (IsCameraLockedToTarget() || (*Target) == nullptr)
	{
		EndTargetLockOn();
		return;
	}

	if (((*Target)->GetActorLocation() - GetOwner()->GetActorLocation()).Size() <= MaxTargetLockDistance)
	{
		CameraTarget = *Target;
	}
}

void UTargetSpringArmComponent::EndTargetLockOn()
{
	if (IsCameraLockedToTarget())
	{
		CameraTarget = nullptr;
	}

	TargetOffset = FVector(0.f, 0.f, 0.f);
}

bool UTargetSpringArmComponent::IsCameraLockedToTarget()
{
	return  CameraTarget != nullptr;
}