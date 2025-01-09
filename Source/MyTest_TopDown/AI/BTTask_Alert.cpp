// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Alert.h"
#include "MyAIController.h"
#include "Monster_Goblin.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Alert::UBTTask_Alert()
{
	NodeName = TEXT("Alert");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Alert::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MyPawn = Cast<AMonster_Goblin>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
		return EBTNodeResult::Failed;

	MyPawn->OnAlert();
	m_bIsAlertEnd = false;


	MyPawn->OnAlertEnd.AddLambda([&]()
		{
			m_bIsAlertEnd = true;
		});


	return EBTNodeResult::InProgress;
}

void UBTTask_Alert::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	if (m_bIsAlertEnd == true)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);	
	}
}

void UBTTask_Alert::CalBehavior()
{
	// 행동을 계산하기 위한 함수 


}
