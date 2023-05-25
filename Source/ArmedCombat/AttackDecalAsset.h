
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/DataAsset.h"

#include "AttackDecalAsset.generated.h"

UCLASS()
class UAttackDecalAsset : public UDataAsset
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Duration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
	float Opacity;
};

UCLASS(Blueprintable)
class URadialAttackDecalAsset : public UAttackDecalAsset
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
	float AttackAngle { 0.5f };

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
	float DynamicRadius { 0.75f };

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "0.5", UIMin = "0", UIMax = "0.5"))
	float OuterRadius { 0.5f };

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "0.5", UIMin = "0", UIMax = "0.5"))
	float InnerRadius { 0.1f };
};


UCLASS(Blueprintable)
class URectAttackDecalAsset : public UAttackDecalAsset
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
	float Heigth;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
	float Width;
};