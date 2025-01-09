// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanAttack.h"
#include "MyAIController.h"
#include "MyTest_TopDownCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Monster_Goblin.h"

UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = Cast<AMonster_Goblin>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentPawn == nullptr)
		return false;
	
	auto Target = Cast<AMyTest_TopDownCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return false;

	if((Target->GetDistanceTo(CurrentPawn) <= 180.f) && (CurrentPawn->GetState() != AMonster_Goblin::EState::Hit))
	{
		float fDistance = Target->GetDistanceTo(CurrentPawn);// Debug

		return bResult;
	}
	else
	{
		return false;
	}



}
