// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AnimInstanceBase.h"
#include "AnimInstance_Goblin.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnDeathPoint);

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UAnimInstance_Goblin : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	UAnimInstance_Goblin();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	// 임시 함수 (DEBUG)
	const UAnimMontage* GetMontageInst(uint8 MontageIndex);

protected:

};
