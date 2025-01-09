// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMyStatComponent::UMyStatComponent()

{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;

	m_BaseStat.Level = 1;
}


// Called when the game starts
void UMyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetLevel(m_BaseStat.Level);
	SetHP(m_BaseStat.HP, m_ModifierStat.HP);
}

void UMyStatComponent::SetLevel(int32 Level)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance != nullptr)
	{
		// Base Setting
		auto StatData = MyGameInstance->GetStatData((Level));
		if (StatData != nullptr)
		{
			m_BaseStat.Level = StatData->Level;
			m_BaseStat.MaxHP = StatData->MaxHP;
			SetHP(m_BaseStat.MaxHP);
			m_BaseStat.MaxMP = StatData->MaxMP;
			SetMP(m_BaseStat.MaxMP);
			m_BaseStat.MaxSP = StatData->MaxSP;
			SetSP(m_BaseStat.MaxSP);

			m_BaseStat.MaxAttack = StatData->MaxAttack;
			m_BaseStat.Attack = m_BaseStat.MaxAttack;
		}
	}
}

void UMyStatComponent::SetHP(int32 Base, int32 Modifier)
{
	m_BaseStat.HP = (Base + Modifier);
	if (m_BaseStat.HP < 0)
	{
		m_BaseStat.HP = 0;
	}
	else if (m_BaseStat.HP > m_BaseStat.MaxHP)
	{
		m_BaseStat.HP = m_BaseStat.MaxHP;
	}

	OnHPChanged.Broadcast();

	if (m_BaseStat.HP == 0)
	{
		OnDeathCheck.Broadcast();
	}

	if (m_BaseStat.HP == m_BaseStat.MaxHP)
	{
		if (m_StatRecovery[0] > 0) { m_StatRecovery[0] *= -1; }
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_HPRecovery);
	}
	else if (m_BaseStat.HP <= m_BaseStat.MaxHP)
	{
		if (m_StatRecovery[0] < 0)
		{
			m_StatRecovery[0] *= -1;
			GetWorld()->GetTimerManager().SetTimer(m_Timer_HPRecovery, this, &UMyStatComponent::Recovery_HP, 3.f, true);
		}
	}
}

void UMyStatComponent::SetMP(int32 Base, int32 Modifier)
{
	m_BaseStat.MP = (Base + Modifier);
	if (m_BaseStat.MP < 0)
	{
		m_BaseStat.MP = 0;
	}
	else if (m_BaseStat.MP > m_BaseStat.MaxMP)
	{
		m_BaseStat.MP = m_BaseStat.MaxMP;
	}

	OnMPChanged.Broadcast();

	if (m_BaseStat.MP == m_BaseStat.MaxMP)
	{
		if (m_StatRecovery[1] > 0) { m_StatRecovery[1] *= -1; }
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_MPRecovery);
	}
	else if (m_BaseStat.MP <= m_BaseStat.MaxMP)
	{
		if (m_StatRecovery[1] < 0)
		{
			m_StatRecovery[1] *= -1;
			GetWorld()->GetTimerManager().SetTimer(m_Timer_MPRecovery, this, &UMyStatComponent::Recovery_MP, 3.f, true);
		}
	}

}

void UMyStatComponent::SetSP(int32 Base, int32 Modifier)
{
	m_BaseStat.SP = (Base + Modifier);
	if (m_BaseStat.SP < 0)
	{
		m_BaseStat.SP = 0;
	}
	else if (m_BaseStat.SP > m_BaseStat.MaxSP)
	{
		m_BaseStat.SP = m_BaseStat.MaxSP;
	}

	OnSPChanged.Broadcast();

	if (m_BaseStat.SP == m_BaseStat.MaxSP)
	{
		if (m_StatRecovery[2] > 0) { m_StatRecovery[2] *= -1; }
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_SPRecovery);
	}
	else if (m_BaseStat.SP <= m_BaseStat.MaxSP)
	{
		if (m_StatRecovery[2] < 0)
		{
			m_StatRecovery[2] *= -1;
			GetWorld()->GetTimerManager().SetTimer(m_Timer_SPRecovery, this, &UMyStatComponent::Recovery_SP, 3.f, true);
		}
	}


}

void UMyStatComponent::OnAttacked(float DamageAmount)
{
	SetHP((m_BaseStat.HP + m_ModifierStat.HP)-DamageAmount);

	UE_LOG(LogTemp, Warning, TEXT("OnAttacked %d"), m_BaseStat.HP + m_ModifierStat.HP);
}

void UMyStatComponent::AddBaseStat(const FMyCharacterData& InAddBaseStat)
{
	m_BaseStat = m_BaseStat + InAddBaseStat; 
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UMyStatComponent::SetBaseStat(const FMyCharacterData& InSetBaseStat)
{
	m_BaseStat = InSetBaseStat;  
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UMyStatComponent::Recovery_HP()
{
	SetHP((m_BaseStat.HP + m_ModifierStat.HP) + m_StatRecovery[0]);
	UE_LOG(LogTemp, Warning, TEXT("Recovery_HP %d"), m_BaseStat.HP + m_ModifierStat.HP);
}

void UMyStatComponent::Recovery_MP()
{
	SetMP((m_BaseStat.MP + m_ModifierStat.MP) + m_StatRecovery[1]);
	UE_LOG(LogTemp, Warning, TEXT("Recovery_MP %d"), m_BaseStat.MP + m_ModifierStat.MP);
}

void UMyStatComponent::Recovery_SP()
{
	SetSP((m_BaseStat.SP + m_ModifierStat.SP) + m_StatRecovery[2]);
	UE_LOG(LogTemp, Warning, TEXT("Recovery_SP %d"), m_BaseStat.SP + m_ModifierStat.SP);
}

