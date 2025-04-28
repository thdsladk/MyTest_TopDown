// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/MyMonsterWidgetInterface.h"
#include "Interface/HighlightInterface.h"
#include "NPCharacter.generated.h"


class AMyEquipment;
class UAnimInstance_Goblin;
class UWidgetComponent;
class UMyUserWidget;
class UUserWidget;
class UInputComponent;
class AController;


UCLASS(config=NPCList)
class MYTEST_TOPDOWN_API ANPCharacter : public ACharacterBase, public IMyMonsterWidgetInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	ANPCharacter(); 

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;


	// Behavior
	virtual void OnIdle()override;
	virtual void OnBattle()override;
	virtual void Attack()override;
	virtual void AttackCheck()override;
	virtual void AttackEnd()override;
	virtual void Death()override;
	virtual void DeathEnd()override;
	virtual void OnHit()override;
	virtual void OnDefense()override;
	virtual void StopDefense()override;
	virtual void Defense_Hit()override;
	virtual void OnDetect()override;
	virtual void OnAlert()override;

	virtual void OnCommand(uint8 Command)override;

	virtual bool IsOnTriggerEscape()override;


	// Highlight Section
public:
	virtual void HighlightActor()override;
	virtual void UnHighlightActor()override;


	// Animation Function			// ��Ÿ���� ���¸� ����ȭ �ع����� ���ӵ� ���赵 ��������.
	UFUNCTION()
	void OnStopAttackMontage(float InBlendOutTime = 0.f);
	UFUNCTION()
	void OnStopDeathMontage(float InBlendOutTime = 0.f);
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnDeathMontageEnded();

	// Damage Section
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual bool IsPlayerCharacter() override{ return false; }

protected:
	// Widget Section
	virtual void SetupMonsterWidget(UMyUserWidget* InUserWidget) override;


protected:
	//S/ �μ� ��Ʈ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCType")
	FString m_NPCType;
	

	// Components

	//UPROPERTY(VisibleAnywhere)
	//class UMonsterStatComponent* m_StatComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimInstance_Goblin> m_AnimInstance;
	UPROPERTY(VisibleAnywhere, Category = "WidgetComp")
	TObjectPtr<UWidgetComponent> m_HpBar;
	UPROPERTY(VisibleAnywhere, Category = "WidgetComp")
	TObjectPtr<UWidgetComponent>m_Emotion;



	// Ŭ������ ���� �����ϰ� �����ؼ� ���� ��� 
	// WidgetComponent�� ���̴� ��İ� �ٸ��� ���  
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> m_DamageFont;


	//E/ �μ� ��Ʈ 

	UPROPERTY()
	double m_AlertTime = -1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AlertTime")
	double m_AlertCoolTime = 5.0;		// �ӽ� �������Ʈ���� ����������.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color" )
	float m_fCurrentOpacity = 1.0f;


protected:
	void NPCMeshLoadCompleted();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;


private:
	FTimerHandle m_hDeathTimer;
	FTimerHandle m_hAlertTimer;

#pragma region DEBUG

private:
	FTimerHandle m_hDebug;
	void Debug();

#pragma endregion

};
