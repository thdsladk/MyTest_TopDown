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

	// ���� ��ü������ ���°� �������� BT������ ������ �ʾƼ� �ι� ȣ���ϴ� ������ �־ ������ 
	// ���°� ���� �������� �����ϵ��� ����.
	// ��� ���� �����鼭 Hit����� ��� �Ǹ� OnHitEnd�� Notify�� ���ϴ� ���� ����. 
	if (m_bIsHit == false )
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("State")), m_NextState);
		m_NextState = 0;	// �ʱ�ȭ
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
