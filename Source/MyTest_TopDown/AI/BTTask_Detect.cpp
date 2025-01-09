// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Detect.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "Monster_Goblin.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_Detect::UBTTask_Detect()
{
	NodeName = TEXT("Detect");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Detect::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Self = Cast<AMonster_Goblin>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return EBTNodeResult::Failed;

	Self->OnDetect();

	m_bIsDetectEnd = false;
	Self->OnDetectEnd.AddLambda([&]()
	{
			m_bIsDetectEnd = true;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Detect::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	if (m_bIsDetectEnd == true)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
