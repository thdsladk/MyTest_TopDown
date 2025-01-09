// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Hit.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UBTTask_Hit : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTask_Hit();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

	bool m_bIsHit = false;
	int32 m_NextState = 0;
};
