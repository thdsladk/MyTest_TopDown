// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnClickBtnQ);
//DECLARE_MULTICAST_DELEGATE(FOnClickBtnW);
//DECLARE_MULTICAST_DELEGATE(FOnClickBtnE);
//DECLARE_MULTICAST_DELEGATE(FOnClickBtnR);
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkill, int8);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	enum ESkill : int8
	{
		Skill_Q,
		Skill_W,
		Skill_E,
		Skill_R,
		END

	};
public:	
	// Sets default values for this component's properties
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void Click_Q();
	void Click_W();
	void Click_E();
	void Click_R();
	void Click_Btn(int8 Btn);

	//FOnClickBtnQ m_OnClickBtnQ;
	//FOnClickBtnW m_OnClickBtnW;
	//FOnClickBtnE m_OnClickBtnE;
	//FOnClickBtnR m_OnClickBtnR;
	//FOnSkill m_OnSkill;

	void Cancel();
	
	void PlaySkill_DoublePain();
	void PlaySkill_Stampede();
	void PlaySkill_Stampede_Knockup();
	void PlaySkill_Cast_WaterBall();
	void PlaySkill_FinalHit();

	// Check Damage
	void CheckSkill_DoublePain();
	void CheckSkill_Stampede();
	void CheckSkill_Stampede_Knockup();
	void CheckSkill_Cast_WaterBall();
	void CheckSkill_FinalHit();

	void Effect_DoublePain();
	void Effect_Thunder();
	void Effect_GhostTrail();

	UFUNCTION()
	void OnSkillMontageEnded();
	UFUNCTION()
	void OnSkillMontagePoint();


protected:

	UPROPERTY()
	TWeakObjectPtr<class AMyTest_TopDownCharacter> m_pOwner;
	UPROPERTY()
	TWeakObjectPtr<class UMyAnimInstance> m_pAnimInstance;
	UPROPERTY()
	TWeakObjectPtr<class UMyStatComponent> m_pStatComp;
		
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Components")
	TSubclassOf<class AGhostTrail> m_pGhostTrail;



	ESkill m_CurrentSkill = ESkill::END;


	// Skill Cooltime 
	FTimerHandle m_Timer_DoublePain;
	FTimerHandle m_Timer_Stampede;
	FTimerHandle m_Timer_FinalHit;
	
	bool m_bStampede = false;
	bool m_bGhostTrail = false;

	// Particle
	UPROPERTY()
	TSubclassOf<AActor> m_ThunderBall;
	UPROPERTY()
	TArray<TSubclassOf<AActor>> m_ThunderList;
	
	UPROPERTY()
	class UNiagaraSystem* m_pDust;

	//UPROPERTY()
	//class UNiagaraSystem* m_SwordTrail;

	// Target<Enemy> List
	UPROPERTY()
	TArray<AActor*> m_TargetList;

	// 내부 기능 함수들 
private:
	bool SearchActor_Sphere(float Range, float Radius, ECollisionChannel Channel);



};
