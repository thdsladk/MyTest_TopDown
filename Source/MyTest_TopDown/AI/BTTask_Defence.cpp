// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Defence.h"
#include "MyAIController.h"
#include "Monster_Goblin.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Defence::UBTTask_Defence()
{
	NodeName = TEXT("Defense");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Defence::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MyPawn = Cast<AMonster_Goblin>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
		return EBTNodeResult::Failed;

	// Set Defense State
	MyPawn->Defense_Hit();
	m_bIsDefense = true;



	MyPawn->OnDefenseEnd.AddLambda([&]()
		{
			m_bIsDefense = false;
		});


	return EBTNodeResult::InProgress;
}

void UBTTask_Defence::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	if (m_bIsDefense == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
