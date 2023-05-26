// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArmedCombatCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "TargetSpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "ArmedCombat\Knight.h"
#include "Math/UnrealMathVectorCommon.h"

//////////////////////////////////////////////////////////////////////////
// AArmedCombatCharacter

AArmedCombatCharacter::AArmedCombatCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	TargetCameraBoom = CreateDefaultSubobject<UTargetSpringArmComponent>(TEXT("TargetCameraBoom"));
	TargetCameraBoom->SetupAttachment(RootComponent);
	TargetCameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
	TargetCameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(TargetCameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AbilitySystemComponent = CreateDefaultSubobject<UArmedAbilitySystemComponent>(TEXT("AbilitySystem"));

	Attributes = CreateDefaultSubobject<UArmAttributeSet>("Attributes");

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AArmedCombatCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	TargetCameraBoom->ToggleLockOn();
}

void AArmedCombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCamera(DeltaTime);
}

void AArmedCombatCharacter::UpdateCamera(float DeltaTime)
{
	if (TargetCameraBoom->IsCameraLockedToTarget())
	{
		FVector targetVector = TargetCameraBoom->CameraTarget->GetActorLocation() - GetActorLocation();
		FVector NewTargetOffset = FMath::VInterpTo(TargetCameraBoom->TargetOffset, targetVector * LockOnCameraOffsetBias, DeltaTime, LockOnTargetOffsetRate);
		TargetCameraBoom->TargetOffset = NewTargetOffset;

		FRotator TargetRotator = targetVector.GetSafeNormal().Rotation();
		TargetRotator.Pitch = PitchBias;
		FRotator CurrentRotator = GetControlRotation();
		FRotator NewRotator = FMath::RInterpTo(CurrentRotator, TargetRotator, DeltaTime, LockOnControlRotationRate);
		GetController()->SetControlRotation(NewRotator);
	}
	else
	{
		FVector NewTargetOffset = FMath::VInterpTo(TargetCameraBoom->TargetOffset, FVector(0.f, 0.f, 0.f), DeltaTime, LockOnTargetOffsetRate);
		TargetCameraBoom->TargetOffset = NewTargetOffset;

		FRotator CurrentRotator = GetControlRotation();
		FRotator TargetRotator = FRotator(PitchBias, CurrentRotator.Yaw, 0.f);
		FRotator NewRotator = FMath::RInterpTo(CurrentRotator, TargetRotator, DeltaTime, FreeCamControlRotationRate);
		GetController()->SetControlRotation(NewRotator);
	}
}



//////////////////////////////////////////////////////////////////////////
// Input

void AArmedCombatCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AArmedCombatCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AArmedCombatCharacter::Look);

		// Lock On Camera // Lock on Always
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Triggered, TargetCameraBoom, &UTargetSpringArmComponent::ToggleLockOn);

		//Dodge
		EnhancedInputComponent->BindAction(DodgeLeftAction, ETriggerEvent::Triggered, this, &AArmedCombatCharacter::DodgeLeft);
		EnhancedInputComponent->BindAction(DodgeRightAction, ETriggerEvent::Triggered, this, &AArmedCombatCharacter::DodgeRight);

		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AArmedCombatCharacter::Attack);


	}
}

void AArmedCombatCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (HasGrabbedKnight && GrabbedKnight)
	{

		PullKnight();
	}
	else
	{
		if (Controller != nullptr)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// get right vector
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void AArmedCombatCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();


	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//////////////////////////////////////////////////////////////////////////
//

UAbilitySystemComponent* AArmedCombatCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AArmedCombatCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle Spechandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect,1,EffectContext);

		if (Spechandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spechandle.Data.Get());
		}
	}
}

void AArmedCombatCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UArmedGameplayAbility>& Ability : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void AArmedCombatCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent->InitAbilityActorInfo(this,this);

	InitializeAttributes();
	GiveAbilities();
}

void AArmedCombatCharacter::DodgeLeft(const FInputActionValue& Value)
{
	if (HasGrabbedKnight && GrabbedKnight)
	{

		PullKnight();
	}
	else
	{
		PlayDodgeAnimations(EDodgeDirection::Left);
		GetMovementComponent()->Velocity = ((-GetFollowCamera()->GetRightVector()) * Attributes->DodgeForce.GetCurrentValue());
	}
}

void AArmedCombatCharacter::DodgeRight(const FInputActionValue& Value)
{
	if (HasGrabbedKnight && GrabbedKnight)
	{

		PullKnight();
	}
	else
	{
		PlayDodgeAnimations(EDodgeDirection::Right);
		GetMovementComponent()->Velocity = ((GetFollowCamera()->GetRightVector()) * Attributes->DodgeForce.GetCurrentValue());
	}
}

void AArmedCombatCharacter::Attack(const FInputActionValue& Value)
{
	if(GetMovementComponent()->IsFalling())
	{
		return;
	}


		GetMovementComponent()->Velocity = ((GetActorForwardVector()) * Attributes->DodgeForce.GetCurrentValue());
		IsAttacking = true;
		GetWorldTimerManager().SetTimer(AttackTimerHandle,this,&AArmedCombatCharacter::StopAttacking, Attributes->AttackLength.GetCurrentValue(),false);

}

void AArmedCombatCharacter::StopAttacking()
{
	if (!HasGrabbedKnight)
	{
		IsAttacking = false;
	}
	AttackTimerHandle.Invalidate();
}

void AArmedCombatCharacter::Jump()
{
	if (HasGrabbedKnight && GrabbedKnight)
	{

		PullKnight();
	}
	else
	{
		ACharacter::Jump();
	}
}

void AArmedCombatCharacter::PullKnight()
{
	GetMovementComponent()->Velocity = (-GetActorForwardVector() * 100);;

	ApplyPullEffectToGrabbedKnight();
}

