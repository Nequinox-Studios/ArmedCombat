#include "TargetSpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Knight.h"

UTargetSpringArmComponent::UTargetSpringArmComponent()
{
	MaxTargetLockDistance = 2000.f;
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

void UTargetSpringArmComponent::ToggleLockOn()
{
	/*if (IsCameraLockedToTarget())
	{
		EndTargetLockOn();
		return;
	}*/

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> TargetActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetOwner()->GetActorLocation(), MaxTargetLockDistance, traceObjectTypes, AKnight::StaticClass(), TArray<AActor*>{GetOwner()}, TargetActors);
	
	for (AActor* Target : TargetActors)
	{
		if ((Target->GetActorLocation() - GetOwner()->GetActorLocation()).Size() <= MaxTargetLockDistance)
		{
			CameraTarget = Target;
			break;
		}
	}
	
}

void UTargetSpringArmComponent::EndTargetLockOn()
{
	if (IsCameraLockedToTarget())
	{
		CameraTarget = nullptr;
	}
}

bool UTargetSpringArmComponent::IsCameraLockedToTarget()
{
	return  CameraTarget != nullptr;
}