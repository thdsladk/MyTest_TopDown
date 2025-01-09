// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToSide.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "Monster_Goblin.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_MoveToSide::UBTTask_MoveToSide()
{
	NodeName = TEXT("MoveToSide");
}

EBTNodeResult::Type UBTTask_MoveToSide::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);


	auto Self = Cast<AMonster_Goblin>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Self)
		return EBTNodeResult::Failed;

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
	{
		Self->SetState(AMonster_Goblin::EState::Idle);
		Self->SetMoveSpeed(600.f);
		return EBTNodeResult::Failed;
	}


	// 랜덤 스트림 생성
	FRandomStream RandomStream(FMath::Rand());
	// 랜덤한 부동 소수점 수를 생성
	float RandomFloat = RandomStream.FRandRange(-1.0f, 1.0f);


	FVector LookVector = Target->GetActorLocation() - Self->GetActorLocation();
	LookVector.Z = 0.0f;
	LookVector.Normalize();
	
	
	float Angle = FMath::Acos(FVector::DotProduct(FVector(1.f,0.f,0.f), LookVector)) / PI;
	
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	TargetRot.Yaw += 90.f;
	
	if (LookVector.X < 0.0)
	{
		Self->LookDirection(Angle);
	
	}
	else
	{
		Self->LookDirection(2.0-Angle);
	}
	
	//Self->SetActorRotation(FMath::RInterpTo(Self->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));
	Self->SetActorRotation(TargetRot);

	// 이동속도 
	Self->SetMoveSpeed(100.f);

	OwnerComp.GetAIOwner()->MoveToLocation(Self->GetActorLocation() +(Self->GetActorForwardVector() * 100.f));


	return EBTNodeResult::Succeeded;
}
