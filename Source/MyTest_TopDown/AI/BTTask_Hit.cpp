// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Hit.h"
#include "MyAIController.h"
#include "Monster_Goblin.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_Hit::UBTTask_Hit()
{
	NodeName = TEXT("Hit");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Hit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MyPawn = Cast<AMonster_Goblin>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
		return EBTNodeResult::Failed;

	// Set Hit State
	//MyPawn->Attacked();
	m_bIsHit = true;
	m_NextState = MyPawn->GetNextState();
	


	MyPawn->OnHitEnd.AddLambda([&]()
		{
			m_bIsHit = false;
		});


	return EBTNodeResult::InProgress;
}

void UBTTask_Hit::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// 몬스터 객체에서는 상태가 변했지만 BT에서는 변하지 않아서 두번 호출하는 문제가 있어서 변경을 
	// 상태가 끝날 시점에서 변경하도록 갱신.
	// 계속 공격 받으면서 Hit모션이 계속 되면 OnHitEnd가 Notify를 못하는 현상 존재. 
	if (m_bIsHit == false )
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("State")), m_NextState);
		m_NextState = 0;	// 초기화
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
