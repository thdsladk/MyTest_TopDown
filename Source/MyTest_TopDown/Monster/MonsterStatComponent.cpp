// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMonsterStatComponent::UMonsterStatComponent()
	:m_Level(1)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	// ...
}


// Called when the game starts
void UMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMonsterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetMonster(TEXT("Goblin-1"));
}

void UMonsterStatComponent::SetMonster(FString Name)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance != nullptr)
	{
		auto StatData = MyGameInstance->GetMonsterStatData(Name);
		if (StatData != nullptr)
		{
			m_Name = StatData->Name;
			m_Type = StatData->Type;
			m_Level = StatData->Level;
			SetHP(StatData->MaxHP);
			m_MaxHP = StatData->MaxHP;
			m_Attack = StatData->Attack;
			m_Defence = StatData->Defence;
		}
	}
}

void UMonsterStatComponent::SetLevel(int32 Level)
{

}

void UMonsterStatComponent::SetHP(int32 HP)
{
	m_HP = HP;
	if (m_HP < 0)
	{
		m_HP = 0;
	}
	else if (m_HP > m_MaxHP)
	{
		m_HP = m_MaxHP;
	}
	
	// 체력 변화 처리 
	OnHPChanged.Broadcast();

	// 죽음 처리 
	if (m_HP == 0)
	{
		OnDeathCheck.Broadcast();
	}

}

void UMonsterStatComponent::OnAttacked(float DamageAmount)
{
	SetHP(m_HP - DamageAmount);

	UE_LOG(LogTemp, Warning, TEXT("OnAttacked %d"), m_HP);
}

