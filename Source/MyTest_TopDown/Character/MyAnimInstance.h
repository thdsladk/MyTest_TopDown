// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AnimInstanceBase.h"
#include "MyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnDeathPoint);
DECLARE_MULTICAST_DELEGATE(FOnSkillPoint);
DECLARE_MULTICAST_DELEGATE(FOnSkillEnd);
DECLARE_MULTICAST_DELEGATE(FOnHitEnd);

/**
 * 
 */


UCLASS()
class MYTEST_TOPDOWN_API UMyAnimInstance : public UAnimInstanceBase
{
	GENERATED_BODY()
public:
	UMyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:

};
