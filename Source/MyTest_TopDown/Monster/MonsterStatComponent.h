// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChanged);
DECLARE_MULTICAST_DELEGATE(FOnDeathCheck);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API UMonsterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;


public:

	void SetMonster(FString Name);
	void SetLevel(int32 Level);
	void SetHP(int32 HP);
	void OnAttacked(float DamageAmount);

	FString GetName() { return m_Name; }
	int32 GetType() { return m_Type; }
	int32 GetLevel() { return m_Level; }
	int32 GetHP() { return m_HP; }
	float GetHPRatio() { return (m_HP / static_cast<float>(m_MaxHP)); }
	int GetAttack() { return m_Attack; }

private:
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		FString m_Name;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		int32 m_Type;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		int32 m_Level;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		int32 m_HP;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		int32 m_MaxHP;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		int32 m_MP;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		int32 m_MaxMP;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		int32 m_SP;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		int32 m_MaxSP;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		int32 m_Attack;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		int32 m_MaxAttack;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		int32 m_Defence;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		int32 m_MaxDefence;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		UStaticMesh* m_LEquipMesh;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
		UStaticMesh* m_REquipMesh;


public:
	FOnHPChanged OnHPChanged;
	FOnDeathCheck OnDeathCheck;

};
