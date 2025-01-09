// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "MyTest_TopDownCharacter.h"
#include "MyAnimInstance.h"
#include "MyStatComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Math/UnrealMathUtility.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

#include "GhostTrail.h"


// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// ThunderBall

	static ConstructorHelpers::FClassFinder<AActor> ThunderBall(TEXT("/Script/Engine.Blueprint'/Game/FXVarietyPack/Blueprints/BP_ky_thunderBall.BP_ky_thunderBall_C'"));
	if (ThunderBall.Succeeded())
	{
		m_ThunderBall = ThunderBall.Class;

	}

	static ConstructorHelpers::FClassFinder<AActor> Thunder(TEXT("/Script/Engine.Blueprint'/Game/FXVarietyPack/Blueprints/BP_ky_lightning1.BP_ky_lightning1_C'"));
	if (Thunder.Succeeded())
	{
		m_ThunderList.Add(Thunder.Class);

	}

	static ConstructorHelpers::FClassFinder<AActor> Thunder_Middle(TEXT("/Script/Engine.Blueprint'/Game/FXVarietyPack/Blueprints/BP_ky_lightning1.BP_ky_lightning2_C'"));
	if (Thunder_Middle.Succeeded())
	{
		m_ThunderList.Add(Thunder_Middle.Class);
	}

	// Niagara System ( Particle ) 
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Dust(TEXT("/Script/Niagara.NiagaraSystem'/Game/A_Surface_Footstep/Niagara_FX/ParticleSystems/PSN_Sand_Surface.PSN_Sand_Surface'"));
	if (Dust.Succeeded())
	{
		m_pDust = Dust.Object;

	}

	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SwordTrail(TEXT("/Script/Niagara.NiagaraSystem'/Game/TopDown/Effect/VFX_SwordTrail.VFX_SwordTrail'"));
	//if (SwordTrail.Succeeded())
	//{
	//	m_SwordTrail = SwordTrail.Object;
	//
	//}

	// GhostTrail
	static ConstructorHelpers::FClassFinder<AActor> GhostTrail(TEXT("/Script/CoreUObject.Class'/Script/MyTest_TopDown.GhostTrail'"));
	if (GhostTrail.Succeeded())
	{
		m_pGhostTrail = GhostTrail.Class;
	}

	/*
	m_pDust = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Dust"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Dust(TEXT("/Script/Niagara.NiagaraSystem'/Game/A_Surface_Footstep/Niagara_FX/ParticleSystems/PSN_Sand_Surface.PSN_Sand_Surface'"));
	if (Dust.Object)
	{
		
		m_pDust->SetAsset(Dust.Object);
		m_pDust->bAutoActivate = false;
		m_pDust->Deactivate();
		m_pDust->SetRelativeScale3D(FVector(1.3f, 1.3f, 1.3f));
	}
	*/
}

// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	m_pOwner = Cast<AMyTest_TopDownCharacter>(GetOwner());
	m_pAnimInstance = Cast<UMyAnimInstance>(m_pOwner.Get()->GetMesh()->GetAnimInstance());
	m_pStatComp = m_pOwner->GetStatComponent();

	// Nullptr Check
	if (m_pOwner == nullptr || m_pAnimInstance == nullptr || m_pStatComp == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT(" Character or AnimInstance or StatComponent is nullptr !!!"));
		return;
	}
	

	// AnimInstance Delegate   // 구독하기.
	m_pAnimInstance->OnSkillEnd.AddUObject(this, &USkillComponent::OnSkillMontageEnded);
	m_pAnimInstance->OnSkillPoint.AddUObject(this, &USkillComponent::OnSkillMontagePoint);




	// etc 


}

void USkillComponent::Click_Q()
{
	if (m_pOwner->GetState() == AMyTest_TopDownCharacter::EState::Idle)
		m_pOwner->SetState(AMyTest_TopDownCharacter::EState::Casting);
	else
		return;

	m_CurrentSkill = ESkill::Skill_Q;

	m_pAnimInstance->PlaySkillMontage();
	m_pAnimInstance->JumpToSection_Skill(TEXT("DoublePain"));


	GetWorld()->GetTimerManager().SetTimer(m_Timer_DoublePain, this, &USkillComponent::PlaySkill_DoublePain, 0.1f, true);

	Effect_DoublePain();
	
	// 제어권 해제
	m_pOwner->DisableInput(Cast<APlayerController>(m_pOwner->GetController()));

}

void USkillComponent::Click_W()
{
	if (m_pOwner->GetState() == AMyTest_TopDownCharacter::EState::Idle)
		m_pOwner->SetState(AMyTest_TopDownCharacter::EState::Casting);
	else
		return;

	m_CurrentSkill = ESkill::Skill_W;

	m_pAnimInstance->PlaySkillMontage();
	m_pAnimInstance->JumpToSection_Skill(TEXT("Stampede"));
	
	//
	m_pOwner->SetSprint(2000.f, 50.f);		// 캐릭터 이동속도
	m_bStampede = true;
	
	// Effect
	GetWorld()->GetTimerManager().SetTimer(m_Timer_Stampede,this,&USkillComponent::Effect_Thunder, 0.1f, true);

	// 제어권 해제
	m_pOwner->DisableInput(Cast<APlayerController>(m_pOwner->GetController()));

}

void USkillComponent::Click_E()
{
	if (m_pOwner->GetState() == AMyTest_TopDownCharacter::EState::Idle)
		m_pOwner->SetState(AMyTest_TopDownCharacter::EState::Casting);
	else
		return;

	m_CurrentSkill = ESkill::Skill_E;

	m_pAnimInstance->PlaySkillMontage();
	m_pAnimInstance->JumpToSection_Skill(TEXT("LevelStart"));

	// 스킬 실행 
	SearchActor_Sphere(1.f, 1200.f, ECollisionChannel::ECC_GameTraceChannel2);

	GetWorld()->GetTimerManager().SetTimer(m_Timer_FinalHit, this, &USkillComponent::PlaySkill_FinalHit, 1.0f, true);
	m_bGhostTrail = true;

	// 제어권 해제
	m_pOwner->DisableInput(Cast<APlayerController>(m_pOwner->GetController()));

}

void USkillComponent::Click_R()
{
	if (m_pOwner->GetState() == AMyTest_TopDownCharacter::EState::Idle)
		m_pOwner->SetState(AMyTest_TopDownCharacter::EState::Casting);
	else
		return;

	m_CurrentSkill = ESkill::Skill_R;

	m_pAnimInstance->PlaySkillMontage();
	m_pAnimInstance->JumpToSection_Skill(TEXT("Cast"));

	// 마법생성 (임시)
	GetWorld()->SpawnActor(m_ThunderBall,&m_pOwner->GetTransform());


	// 제어권 해제
	m_pOwner->DisableInput(Cast<APlayerController>(m_pOwner->GetController()));

}

void USkillComponent::Click_Btn(int8 Btn)
{
	switch (Btn)
	{
	case ESkill::Skill_Q :
	{
		Click_Q();
		break;
	}
	case ESkill::Skill_W:
	{
		Click_W();
		break;
	}
	case ESkill::Skill_E:
	{
		Click_E();
		break;
	}
	case ESkill::Skill_R:
	{
		Click_R();
		break;
	}
	case ESkill::END:
	{
		break;
	}
	}
}

void USkillComponent::Cancel()
{
	// 상태가 취소될때. 
	m_CurrentSkill = ESkill::END;
	m_bStampede = false;
	m_bGhostTrail = false;

	GetWorld()->GetTimerManager().ClearTimer(m_Timer_Stampede);
	m_pOwner->StopSprint();
}

void USkillComponent::PlaySkill_DoublePain()
{


	

}

void USkillComponent::PlaySkill_Stampede()
{
	FVector WorldDirection = m_pOwner->GetActorForwardVector().GetSafeNormal();
	m_pOwner->AddMovementInput(WorldDirection, 1.0f, false);
	

}

void USkillComponent::PlaySkill_Stampede_Knockup()
{
	//m_CurrentSkill = ESkill::END;	// END상태로 돌리면 위험할듯하니 조정하자 .

	GetWorld()->GetTimerManager().ClearTimer(m_Timer_Stampede);

	m_pAnimInstance->PlaySkillMontage();
	m_pAnimInstance->JumpToSection_Skill(TEXT("Stampede_Knockup"));



}

void USkillComponent::PlaySkill_Cast_WaterBall()
{


}

void USkillComponent::PlaySkill_FinalHit()
{
	// 한번의 공격 루틴 
	
	// 리스트 확인
	if (m_TargetList.IsEmpty())
	{
		m_pOwner->EnableInput(Cast<APlayerController>(m_pOwner->GetController()));
		m_CurrentSkill = ESkill::END;
		m_pOwner->SetState(AMyTest_TopDownCharacter::EState::Idle);
		m_bGhostTrail = false;
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_FinalHit);
		return;
	}

	AActor* pTarget = m_TargetList.Last();
	if (pTarget == nullptr)
	{
		return;
	}

	// 이동 

	m_pOwner->SetActorLocation(pTarget->GetActorLocation() + (pTarget->GetActorForwardVector() * 50.f));
	
	FVector LookVector = -(pTarget->GetActorForwardVector());
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	m_pOwner->SetActorRotation(TargetRot);

	// 공격 

	m_pOwner->Attack();

	// 대기

	m_TargetList.Remove(pTarget);


}

void USkillComponent::CheckSkill_DoublePain()
{
	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams Params(NAME_None, false, m_pOwner.Get());

	float AttackRange = 150.f;
	float AttackRadius = 200.f;
	FVector ActorLocation = m_pOwner->GetActorLocation();
	FVector ActorRigthVector = m_pOwner->GetActorRightVector();

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OUT OverlapResult,
		ActorLocation + ActorRigthVector * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	FVector Vec = ActorRigthVector * AttackRange;
	FVector Center = ActorLocation + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
	//	Rotation, DrawColor, false, 2.f);

	if (bResult)
	{
		for (FOverlapResult Result : OverlapResult)
		{
			if (Result.GetActor() == nullptr)
				continue;

			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *Result.GetActor()->GetName());
			FDamageEvent DamageEvent;

			int32 Damage = m_pStatComp->GetAttack() * 20;
			Result.GetActor()->TakeDamage(Damage, DamageEvent, m_pOwner->GetController(), m_pOwner.Get());
			
		}
		
	}
}

void USkillComponent::CheckSkill_Stampede()
{
}

void USkillComponent::CheckSkill_Stampede_Knockup()
{
	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams Params(NAME_None, false, m_pOwner.Get());

	float AttackRange = 150.f;
	float AttackRadius = 200.f;
	FVector ActorLocation = m_pOwner->GetActorLocation();
	FVector ActorRigthVector = m_pOwner->GetActorRightVector();

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OUT OverlapResult,
		ActorLocation + ActorRigthVector * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	FVector Vec = ActorRigthVector * AttackRange;
	FVector Center = ActorLocation + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
	//	Rotation, DrawColor, false, 2.f);

	if (bResult)
	{
		for (FOverlapResult Result : OverlapResult)
		{
			if (Result.GetActor() == nullptr)
				continue;

			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *Result.GetActor()->GetName());
			FDamageEvent DamageEvent;
			Result.GetActor()->TakeDamage(m_pStatComp->GetAttack(), DamageEvent, m_pOwner->GetController(), m_pOwner.Get());

		}

	}
}

void USkillComponent::CheckSkill_Cast_WaterBall()
{
}

void USkillComponent::CheckSkill_FinalHit()
{
}

void USkillComponent::Effect_DoublePain()
{
	// SwordTrail Effect 

	//UNiagaraFunctionLibrary::SpawnSystemAttached(
	//	m_SwordTrail,  // 사용할 검흔 효과의 Niagara 에셋 이름을 넣어주세요.
	//	m_pOwner->GetMesh(),
	//	FName(TEXT("FX_Trail_L_01")),
	//	FVector::ZeroVector,
	//	FRotator::ZeroRotator,
	//	EAttachLocation::KeepRelativeOffset,
	//	true
	//);
	//
	//UNiagaraFunctionLibrary::SpawnSystemAttached(
	//	m_SwordTrail,  // 사용할 검흔 효과의 Niagara 에셋 이름을 넣어주세요.
	//	m_pOwner->GetMesh(),
	//	FName(TEXT("FX_Trail_R_01")),
	//	FVector::ZeroVector,
	//	FRotator::ZeroRotator,
	//	EAttachLocation::KeepRelativeOffset,
	//	true
	//);



	


}

void USkillComponent::Effect_Thunder()
{
	
	int32 nMax = 20;
	int32 nMin = 5;
	for (auto elem : m_ThunderList)
	{
		if (nMax < nMin)
			nMax = nMin;
		int32 Count = FMath::RandRange(nMin, nMax -nMin);
		nMax -= Count;
		
		for (int32 i = 0; i < Count; i++)
		{
			float fRangeX = FMath::RandRange(-10.f, 10.f) * 40.f;
			float fRangeY = FMath::RandRange(-10.f, 10.f) * 40.f;

			FTransform Transform = m_pOwner->GetTransform();
			Transform.SetLocation(Transform.GetLocation() + FVector(fRangeX, fRangeY,0.f ));
			GetWorld()->SpawnActor(elem, &Transform);
		}

	}
	

	
	// Dust Effect 

	UNiagaraFunctionLibrary::SpawnSystemAttached(
		m_pDust,  // 사용할 먼지 효과의 Niagara 에셋 이름을 넣어주세요.
		m_pOwner->GetMesh(),
		FName(TEXT("foot_l")),  // 이름은 필요에 따라 변경 가능
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true
	);

	UNiagaraFunctionLibrary::SpawnSystemAttached(
		m_pDust,  // 사용할 먼지 효과의 Niagara 에셋 이름을 넣어주세요.
		m_pOwner->GetMesh(),
		FName(TEXT("foot_R")),  // 이름은 필요에 따라 변경 가능
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true
	); 
	

	// GhostTrail Effect

	FVector Location = m_pOwner->GetActorLocation();

	AGhostTrail* GhostTrail = Cast<AGhostTrail>(GetWorld()->SpawnActor(m_pGhostTrail, &Location));
	GhostTrail->SetupGhostTrail(m_pOwner.Get());
	GhostTrail->BeginGhostTrailEffect();

	
}

void USkillComponent::Effect_GhostTrail()
{
	FVector Location = m_pOwner->GetActorLocation();

	AGhostTrail* GhostTrail = Cast<AGhostTrail>(GetWorld()->SpawnActor(m_pGhostTrail, &Location));
	GhostTrail->SetupGhostTrail(m_pOwner.Get());
	GhostTrail->BeginGhostTrailEffect();

}

void USkillComponent::OnSkillMontageEnded()
{

	m_pOwner->EnableInput(Cast<APlayerController>(m_pOwner->GetController()));

	//m_pAnimInstance->Montage_Stop(0.f);

	switch (m_CurrentSkill)
	{
	case USkillComponent::Skill_Q:
	{

		m_CurrentSkill = ESkill::END;
		m_pOwner->SetState(AMyTest_TopDownCharacter::EState::Idle);
		GetWorld()->GetTimerManager().ClearTimer(m_Timer_DoublePain);
		break;
	}
	case USkillComponent::Skill_W:
	{
		m_pOwner->StopSprint();
		m_bStampede = false;
		PlaySkill_Stampede_Knockup();
		break;
	}
	case USkillComponent::Skill_E:
	{
		//m_CurrentSkill = ESkill::END;
		//m_pOwner->SetState(AMyTest_TopDownCharacter::EState::Idle);
		break;
	}
	case USkillComponent::Skill_R:
	{
		m_CurrentSkill = ESkill::END;
		m_pOwner->SetState(AMyTest_TopDownCharacter::EState::Idle);
		break;
	}
	case USkillComponent::END:
	{
		m_pOwner->SetState(AMyTest_TopDownCharacter::EState::Idle);
		break;
	}
	default:
	{
		m_CurrentSkill = ESkill::END;
		m_pOwner->SetState(AMyTest_TopDownCharacter::EState::Idle);
		break;
	}
	}


}

void USkillComponent::OnSkillMontagePoint()
{

	switch (m_CurrentSkill)
	{
	case USkillComponent::Skill_Q:
	{
		CheckSkill_DoublePain();
		break;
	}
	case USkillComponent::Skill_W:
	{
		CheckSkill_Stampede_Knockup();
		m_CurrentSkill = ESkill::END;		// END로 전환해서  애니메이션이 끝났을때는 
		break;								// OnSkillMontateEnded 에서 Skill_W가 두번 실행 안되도록 한다.
	}
	case USkillComponent::Skill_E:
	{

		break;
	}
	case USkillComponent::Skill_R:
	{

		break;
	}
	case USkillComponent::END:
	{

		break;
	}
	default:
	{

		break;
	}
	}

}

bool USkillComponent::SearchActor_Sphere(float Range, float Radius, ECollisionChannel Channel)
{
	if (m_TargetList.Num() > 0)
	{
		return false;
	}

	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams Params(NAME_None, false, m_pOwner.Get());

	FVector ActorLocation = m_pOwner->GetActorLocation();
	FVector ActorRigthVector = m_pOwner->GetActorRightVector();

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OUT OverlapResult,
		ActorLocation,
		FQuat::Identity,
		Channel,
		FCollisionShape::MakeSphere(Radius),
		Params);

	FVector Vec = ActorRigthVector * Range;
	FVector Center = ActorLocation + Vec * 0.5f;
	float HalfHeight = Range * 0.5f + Radius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Blue;
	else
		DrawColor = FColor::Purple;

	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, Radius,
	//	Rotation, DrawColor, false, 2.f);

	if (bResult)
	{
		for (FOverlapResult Result : OverlapResult)
		{
			if (Result.GetActor() == nullptr)
				continue;

			UE_LOG(LogTemp, Log, TEXT("Search Actor : % s"), *Result.GetActor()->GetName());
			m_TargetList.Add(Result.GetActor());
		}

	}

	return bResult;
}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (m_bStampede)
	{
		PlaySkill_Stampede();
	}

	if (m_bGhostTrail)
	{
		Effect_GhostTrail();
	}
}


/*

/Script/Engine.ParticleSystem'/Game/ParagonGrux/FX/Particles/Abilities/Ultimate/FX/P_MeleeTrails_Ultimate.P_MeleeTrails_Ultimate'

*/