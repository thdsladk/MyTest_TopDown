// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "MyTest_TopDownCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UMyAnimInstance;
class UMyInventoryComponent;
class USkillComponent;
class UInputComponent;
class AMyInteractable;
class AController;


UCLASS(Blueprintable)
class AMyTest_TopDownCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AMyTest_TopDownCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
public:
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void ChangeCameraView();

	/** Returns m_TopDownCameraComp subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return m_TopDownCameraComp; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return m_CameraBoom; }

// Components Part
public:

	USkillComponent& GetSkillComponent() { return *(m_pSkillComp.Get()); }
protected:
	UMyInventoryComponent* GetInventoryComponent() { return m_pInventoryComp.Get(); }
	UMyInventoryComponent& GetInventoryComponent_Ref() { return *(m_pInventoryComp.Get()); }

public:
	// Behavior Section
	virtual void Attack()override;
	virtual void AttackCheck()override;
	virtual void AttackEnd()override;
	virtual void OnHit()override;
	virtual void Death()override;


	// ArmCamera Section 
	void Look_UpDown(float Value);
	void Look_LeftRight(float Value);
	void Wheel_UpDown(float value);


	// Movement Function (override)
	virtual void MoveToFoward(float Value)override;
	virtual void MoveToRight(float Value)override;

	virtual	void Sprint()override;
	virtual	void SetSprint(float WalkSpeed , float CameraSpeed)override;
	virtual	void StopSprint()override;
	
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


	// Interact Section 
	void Click_F();
	void CheckForInteractable();


	UFUNCTION()
		bool HasCurrentInteractable();
	UFUNCTION()
		void SetCurrentInteractable(AMyInteractable* Interactable);

	
	// Damage Section
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool IsPlayerCharacter() override { return true; }

#pragma region Components Member

protected:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> m_TopDownCameraComp;
	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> m_CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = Components)
	TObjectPtr<UMyAnimInstance> m_pAnimInstance;
	UPROPERTY(VisibleAnywhere, Category = Components)
	TObjectPtr<USkillComponent> m_pSkillComp;

#pragma endregion

protected:


	// Interactable Section
	UPROPERTY()
	float m_Reach{1000.f};
	UPROPERTY()
	TWeakObjectPtr<AMyInteractable> m_CurrentInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString m_HelpText;

	UPROPERTY()
	bool m_bShoulderView = false;

	// Timer 
	FTimerHandle m_Timer_ItemCollision;

#pragma region DEBUG

private:
	FTimerHandle m_hDebug;
	void Debug();

#pragma endregion

};

