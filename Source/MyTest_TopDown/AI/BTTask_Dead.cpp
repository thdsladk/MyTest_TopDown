// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Dead.h"
#include "MyAIController.h"
#include "Monster_Goblin.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_Dead::UBTTask_Dead()
{
	NodeName = TEXT("Dead");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Dead::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MyPawn = Cast<AMonster_Goblin>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
		return EBTNodeResult::Failed;

	// 죽음 상태는 단 한번만 이루어지니까. 
	//m_bIsDeadEnd = false;

	MyPawn->OnDeathEnd.AddLambda([&]()
		{
			m_bIsDeadEnd = true;
		});

	//OwnerComp.PauseLogic(TEXT("Paused by Monster"));
	

	return EBTNodeResult::InProgress;
	//return Result;
}

void UBTTask_Dead::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (m_bIsDeadEnd == true)
	{
		//OwnerComp.ResumeLogic(TEXT("Resumed by Monster"));
		//CastChecked<AMyAIController>(OwnerComp.GetAIOwner())->StopAI();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}


}
