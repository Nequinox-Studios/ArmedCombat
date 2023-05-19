// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
//#include "GameplayEffectTypes.h"

#include "ArmedCombatCharacter.generated.h"

UCLASS(config=Game)
class AArmedCombatCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UTargetSpringArmComponent* TargetCameraBoom;
	
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Lock Camera on Target Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LockOnAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UArmedAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UArmAttributeSet* Attributes;

public:
	/*UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<UArmedGameplayAbility>> DefaultAbilities;*/

	/** When true, player wants to Lock On nearby Target */
	UPROPERTY(BlueprintReadOnly, Category = Character)
	uint32 bPressedLockOn:1;

	/** Target Actor for camera lock */
	UPROPERTY(EditAnywhere, Category = "Target Lock Camera")
	AActor* CameraTarget { nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target Lock Camera")
	float LockOnControlRotationRate { 10.f };
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target Lock Camera")
	float LockOnTargetOffsetRate { 10.f };

public:
	AArmedCombatCharacter();

	virtual void Tick(float DeltaTime) override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	// Update camera position and focus
	void UpdateCamera(float DeltaTime);

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAttributes();
	virtual void GiveAbilities();

	virtual void PossessedBy(AController* NewController) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class UTargetSpringArmComponent* GetCameraBoom() const { return TargetCameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

