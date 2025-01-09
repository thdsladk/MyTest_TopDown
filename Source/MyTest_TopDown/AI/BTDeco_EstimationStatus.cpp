// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDeco_EstimationStatus.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster_Goblin.h"
#include "MonsterStatComponent.h"

UBTDeco_EstimationStatus::UBTDeco_EstimationStatus()
{
	NodeName = TEXT("EstimationStatus");
}

bool UBTDeco_EstimationStatus::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto Self = Cast<AMonster_Goblin>(OwnerComp.GetAIOwner()->GetPawn());
	if (Self == nullptr)
		return false;

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return false;


	if (0.f < Self->GetStatus()->GetHPRatio() &&Self->GetStatus()->GetHPRatio() < 0.2f )
	{
		return bResult;
	}
	else
	{
		return false;
	}

}
