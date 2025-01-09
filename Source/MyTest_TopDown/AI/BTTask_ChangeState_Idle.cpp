// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeState_Idle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "Monster_Goblin.h"
#include "MyTest_TopDownCharacter.h"

UBTTask_ChangeState_Idle::UBTTask_ChangeState_Idle()
{
	NodeName = TEXT("ChangeStateIdle");
}

EBTNodeResult::Type UBTTask_ChangeState_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Self = Cast<AMonster_Goblin>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return EBTNodeResult::Failed;

	auto Target = Cast<AMyTest_TopDownCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
	{
		Self->SetState(AMonster_Goblin::EState::Idle);
		return Result;
	}

	return Result;





}
