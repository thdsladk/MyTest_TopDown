// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "MyStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHPChanged);
DECLARE_MULTICAST_DELEGATE(FOnMPChanged);
DECLARE_MULTICAST_DELEGATE(FOnSPChanged);
DECLARE_MULTICAST_DELEGATE(FOnSPChanged);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, const FMyCharacterData& /*BaseStat*/, const FMyCharacterData& /*ModifierStat*/);


DECLARE_MULTICAST_DELEGATE(FOnDeathCheck);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API UMyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	void SetLevel(int32 Level);
	void SetHP(int32 Base ,int32 Modifier = 0);
	void SetMP(int32 Base ,int32 Modifier = 0);
	void SetSP(int32 Base ,int32 Modifier = 0);

	void OnAttacked(float DamageAmount);

	int32 GetLevel() { return m_BaseStat.Level + m_ModifierStat.Level; }
	int32 GetHP() { return m_BaseStat.HP + m_ModifierStat.HP; }
	int32 GetMP() { return m_BaseStat.MP + m_ModifierStat.MP; }
	int32 GetSP() { return m_BaseStat.SP + m_ModifierStat.SP; }

	float GetHPRatio() { return ((m_BaseStat.HP + m_ModifierStat.HP) / static_cast<float>(m_BaseStat.MaxHP + m_ModifierStat.MaxHP)); }
	//int32 GetHPRatio() { return ((m_HP * 100) / m_MaxHP); } // 비율을 넣으면 잡아주는 함수를 쓴다 .
	float GetMPRatio() { return ((m_BaseStat.MP + m_ModifierStat.MP) / static_cast<float>(m_BaseStat.MaxMP + m_ModifierStat.MaxMP));}
	float GetSPRatio() { return ((m_BaseStat.SP + m_ModifierStat.SP) / static_cast<float>(m_BaseStat.MaxSP + m_ModifierStat.MaxSP));}

	int32 GetAttack() { return m_BaseStat.Attack + m_ModifierStat.Attack; }

	FORCEINLINE const FMyCharacterData& GetBaseStat() { return m_BaseStat; };
	FORCEINLINE	const FMyCharacterData& GetModifierStat() { return m_ModifierStat; }

	void AddBaseStat(const FMyCharacterData& InAddBaseStat); 
	void SetBaseStat(const FMyCharacterData& InSetBaseStat); 
protected:

	void Recovery_HP();
	void Recovery_MP();
	void Recovery_SP();

private:
	// Stat
	//UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	//int32 m_Level;
	//UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	//int32 m_HP;						   
	//UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	//int32 m_MaxHP;					   
	//UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	//int32 m_MP;						   
	//UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	//int32 m_MaxMP;					   
	//UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	//int32 m_SP;						   
	//UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	//int32 m_MaxSP;					   
	//UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	//int32 m_Attack;					   
	//UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	//int32 m_MaxAttack;


	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	FMyCharacterData m_BaseStat;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	FMyCharacterData m_ModifierStat;



public:

	FOnHPChanged OnHPChanged;
	FOnMPChanged OnMPChanged;
	FOnSPChanged OnSPChanged;
	FOnDeathCheck OnDeathCheck;
	FOnStatChanged OnStatChanged;

	// timer
	int32 m_StatRecovery[3]{-1,-1,-1};
	
	FTimerHandle m_Timer_HPRecovery;
	FTimerHandle m_Timer_MPRecovery;
	FTimerHandle m_Timer_SPRecovery;



};



/*

스텟 자연 회복 시스템은 m_StateRecovery 값이 음수면 비활성화 상태 양수면 활성화상태



*/