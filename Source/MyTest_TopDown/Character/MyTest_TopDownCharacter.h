// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyTest_TopDownCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnDeathEnd);



UCLASS(Blueprintable)
class AMyTest_TopDownCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	enum EState : int8
	{
		Idle,
		Walk,
		Running,
		Attacking,
		Hit,
		Stunned,
		Casting,
		Die,
		End

	};

public:
	AMyTest_TopDownCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
public:
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ChangeCameraView();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

// Components Part
public:
	class UMyInventoryComponent* GetInventoryComponent() { return m_pInventoryComp; }
	class USkillComponent* GetSkillComponent() { return m_pSkillComp; }
	class UMyStatComponent* GetStatComponent() { return m_pStatComp; }

public:
	// Behavior Section
	void Attack();
	void AttackCheck();
	void Attack_End();
	void Attacked();

	void Death();

	FOnAttackEnd OnAttackEnd;
	FOnDeathEnd OnDeathEnd;

	// State Section
	int8 GetState() { return m_CharacterState; }
	void SetState(int8 State) { m_CharacterState = State; }

	// Transform Section 
	void UpDown(float Value);
	void LeftRight(float Value);
	void UpDown_Keyboard(float Value);
	void LeftRight_Keyboard(float Value);

	void Yaw(float value);

	UFUNCTION()
	float GetHorizontal() { return m_LeftRightValue; }
	UFUNCTION()
	float GetVertical() { return m_UpDownValue; }

	// Movement Section
	UFUNCTION()
		void Sprint();
	UFUNCTION()
		void SetSprint(float WalkSpeed , float CameraSpeed);
	UFUNCTION()
		void StopSprint();
	
	// HUD Section 
	UFUNCTION()
	void HUDUpdate(uint8 Type);

	// Animation Section
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnDeathMontageEnded();
	UFUNCTION()
	void OnHitMontageEnded();

	// Inventory Section
	UFUNCTION()
	class UMyInventoryComponent* GetInventory() { return m_pInventoryComp; }

	// Interact Section 
	void Interact();
	void CheckForInteractable();

	void UseItem(int32 Index);
	void DropItem(int32 Index,FVector Pos = FVector(0.0,0.0,0.0));
	UFUNCTION()
		bool HasCurrentInteractable();
	UFUNCTION()
		void SetCurrentInteractable(class AMyInteractable* Interactable);

	
	// Damage Section
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
#pragma region Components

protected:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;
	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere)
		class UMyStatComponent* m_pStatComp;
	UPROPERTY(VisibleAnywhere)
		class UMyInventoryComponent* m_pInventoryComp;
	UPROPERTY(VisibleAnywhere)
		class USkillComponent* m_pSkillComp;
	UPROPERTY()
		class UMyAnimInstance* m_pAnimInstance;


#pragma endregion

protected:

	UPROPERTY()
	int32 m_AttackIndex = 0;
	UPROPERTY()
	float m_UpDownValue = 0.f;
	UPROPERTY()
	float m_LeftRightValue = 0.f;

	// Combat Stat 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float m_AttackRange = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float m_AttackRadius = 150.f;

	// FSM 
	UPROPERTY(VisibleAnywhere, Category = "State")
	int8 m_CharacterState = EState::Idle;

	// Interactable Section
	UPROPERTY()
	float m_Reach =1000.f;
	UPROPERTY()
	TWeakObjectPtr<class AMyInteractable> m_CurrentInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString m_HelpText;

	UPROPERTY()
	bool m_bShoulderView = false;

	// Timer 
	FTimerHandle m_Timer_ItemCollision;


};

