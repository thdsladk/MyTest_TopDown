// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMyAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void RunAI();
	void StopAI();

	bool AddCommandQueue(uint8 Command);
	bool UseCommandQueue(uint8& Command);
	bool IsEmptyCommandQueue();


	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;


private:
	void RandomMove();
private:
	FTimerHandle TimerHandle;

	UPROPERTY()
	class UBehaviorTree* BehaviorTree;
	UPROPERTY()
	class UBlackboardData* BlackboardData;

protected:
	TQueue<uint8> m_CommandQueue;


};
