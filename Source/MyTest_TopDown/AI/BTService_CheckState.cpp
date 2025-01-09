// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckState.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster_Goblin.h"
#include "MonsterStatComponent.h"
//#include "DrawDebugHelpers.h"



UBTService_CheckState::UBTService_CheckState()
{
	NodeName = TEXT("SV_CheckState");
	Interval = 1.0f;
}


void UBTService_CheckState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto Self = Cast<AMonster_Goblin>(OwnerComp.GetAIOwner()->GetPawn());
	if (Self == nullptr)
		return;

	FString Str[]
	{
		TEXT("Idle"),
		TEXT("Walk"),
		TEXT("Running"),
		TEXT("Chase"),
		TEXT("Attacking"),
		TEXT("Attacking_End"),
		TEXT("Hit"),
		TEXT("Defence"),
		TEXT("Stunned"),
		TEXT("Detect"),
		TEXT("Battle"),
		TEXT("Alert"),
		TEXT("Die"),
		TEXT("End")
	};

	
	int32 State = Self->GetState();
	int32 PreState = 0;
	//Debug
	if (State != PreState)
	{
		UE_LOG(LogTemp, Log, TEXT("MonState : %s"), *Str[State]);
		PreState = State;
	}

	if (PreState == AMonster_Goblin::EState::Die)
	{
		// 죽음 상태에서는 항상 die로만 귀결되도록 
		State = PreState;
	}


	switch (State)
	{
		case AMonster_Goblin::EState::Idle :
		{
			Self->SetMoveSpeed(400.f);
			Self->LookDirection(0.f);
			break;
		}
		case AMonster_Goblin::EState::Die :
		{

			break;
		}
		case AMonster_Goblin::EState::Hit :
		{
			break;
		}
		case AMonster_Goblin::EState::Walk:
		{
			break;
		}
		case AMonster_Goblin::EState::Stunned:
		{
			break;
		}
		case AMonster_Goblin::EState::Detect :
		{
			break;
		}
		case AMonster_Goblin::EState::Battle :
		{
			Self->SetMoveSpeed(300.f);
			Self->LookDirection(0.f);
			break;
		}
		case AMonster_Goblin::EState::Alert :
		{
			Self->SetMoveSpeed(200.f);

			break;
		}
		case AMonster_Goblin::EState::End:
		{
			break;
		}

		default :
		{
			break;
		}


	}
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("State")), State);




}
