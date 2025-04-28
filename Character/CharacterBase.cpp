// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Component Section
#include "CharacterComponent/MyInventoryComponent.h"
#include "CharacterComponent/MyEquipmentComponent.h"
#include "CharacterComponent/MyStatComponent.h"

#include "Header/GlobalEnum.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Combat Stat    // ���߿� �����͸� �޾Ƽ� �ʱ�ȭ �� ���̶� 0.f�� �ʱ�ȭ
	m_AttackRange = 0.f;
	m_AttackRadius = 0.f;
	m_DefenseRadius = 0.f;
	m_DetectionRadius = 0.f;
	m_VisibleRadius = 0.f;


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->JumpZVelocity = 800.f; // 600.f;

	// Speed Set ( MaxWalkSpeed : 600.f )
	m_OriginMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;


	// Set Components
	//m_StatComp = CreateDefaultSubobject<UMyStatComponent>(TEXT("Stat"));


	static ConstructorHelpers::FObjectFinder<UMaterial> Shiny(TEXT("/Game/TopDown/Material/M_Shiny.M_Shiny"));
	if (Shiny.Succeeded())
	{
		m_Shiny = Shiny.Object;
	}

	// Stat
	m_StatComp = CreateDefaultSubobject<UMyStatComponent>(TEXT("StatComp"));
	m_pInventoryComp = CreateDefaultSubobject<UMyInventoryComponent>(TEXT("Inventory"));
	m_pEquipmentComp = CreateDefaultSubobject<UMyEquipmentComponent>(TEXT("Equip"));

	m_EquipmentMeshComp.Init(nullptr, m_ItemFirstIndex);

	// ��� ����
	for (int32 Index = 0; Index < m_ItemFirstIndex; Index++)
	{
		// Equipment Component
		m_EquipmentMeshComp[Index] = CreateDefaultSubobject<USkeletalMeshComponent>(m_PartTypeList[Index]);
	}

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	

	// OriginMaterial Init Setting
	m_OriginMaterial = GetMesh()->GetMaterial(0);

	//(�ӽ�) ��� �޽� ������Ʈ ����. 
	m_pInventoryComp->BindEquipment(m_EquipmentMeshComp);



}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (CharacterMesh == nullptr)
	{
		return;
	}

	// ��� �ʱ�ȭ 
	for (int32 Index = 0; Index < m_ItemFirstIndex; Index++)
	{
		// Equipment Component

		if (CharacterMesh->DoesSocketExist(m_PartTypeList[Index]) == true)
		{
			// ������ �𸣰����� SetupAttachment�� �۵��� �ȵȴ�.
			//m_EquipmentMeshComp[Index]->SetupAttachment(CharacterMesh, m_PartTypeList[Index]);
			if (m_EquipmentMeshComp[Index]->AttachToComponent
			(
				CharacterMesh,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				m_PartTypeList[Index]
			) == false)
			{
				UE_LOG(LogTemp, Log, TEXT(" Not Attached "));
			}

			// ����� �����Ǵ��� �α� �����.

			if (GetMesh() == CastChecked<USkeletalMeshComponent>(m_EquipmentMeshComp[Index]->GetAttachParent()))
			{
				UE_LOG(LogTemp, Log, TEXT("Equipment successfully attached to character!"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Attachment failed!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Not Found Socket Name!!!"));
		}



		// ����� Load�ϰ� �ִ� ���â�� ������, Load�Ŀ� �ʱ� ���� �������ִ� �κ�.
		/*
		AMyEquipment* Equipment = Cast<AMyEquipment>(m_Inventory[Index]);
		if (Equipment != nullptr)
		{
			m_EquipmentMeshComp[Index]->SetSkeletalMesh(Equipment->GetEquipmentMesh());
		}
		*/

	}


}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::Effect_Flick(FLinearColor Color)
{
	if (true == GetWorld()->GetTimerManager().IsTimerActive(m_hEffectFlickTimer))
		return;

	GetWorld()->GetTimerManager().SetTimer(m_hEffectFlickTimer, this, &ACharacterBase::EffectEnd_Flick, 0.1f, false);

	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(m_Shiny, this);
	DynamicMaterial->SetVectorParameterValue(FName("Param_1"), Color);

	GetMesh()->SetMaterial(0, DynamicMaterial);

	//DEBUG

	//bool OK = false;
	//if (DynamicMaterial->IsVectorParameterUsedAsChannelMask(TEXT("Param_1"), OK) == true)
	//{ UE_LOG(LogTemp, Log, TEXT("ColorName : OK ")); }
	//else 
	//{ UE_LOG(LogTemp, Log, TEXT("ColorName : Failed ")); }
}

void ACharacterBase::EffectEnd_Flick()
{
	GetMesh()->SetMaterial(0, m_OriginMaterial);
}

void ACharacterBase::MoveToFoward(float Value)
{
	// ���� ������ �ִϸ��̼� ������ ������ ���� ������
	m_Vertical = FMath::Clamp(Value, -1.f, 1.f);
	AddMovementInput(GetActorForwardVector(), Value);
}

void ACharacterBase::MoveToRight(float Value)
{
	// ���� ������ �ִϸ��̼� ������ ������ ���� ������
	m_Horizontal = FMath::Clamp(Value, -1.f, 1.f);
	AddMovementInput(GetActorRightVector(), Value);
}

void ACharacterBase::SetMoveSpeed(float WalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// ĳ���� �̵��ӵ�
}

void ACharacterBase::ResetMoveSpeed()
{
	// �����ε带 �����ʰ� ��������� Reset�̶�� �ܾ�� �������� ����Ͽ���.
	GetCharacterMovement()->MaxWalkSpeed = m_OriginMoveSpeed;
}

void ACharacterBase::SetMoveSpeed_Ratio(float Ratio)
{	
	// ���� 0.f~100.f���� �����ϱ� ���ؼ� Clamp���ش�.
	GetCharacterMovement()->MaxWalkSpeed = m_OriginMoveSpeed * FMath::Clamp(Ratio, 0.f, 100.f);
}

void ACharacterBase::Sprint()
{
	if (m_CharacterState == EBehaviorState::Idle || m_CharacterState == EBehaviorState::Battle)
	{
		m_CharacterState = EBehaviorState::Running;
		GetCharacterMovement()->MaxWalkSpeed = (m_OriginMoveSpeed * 2.f);		// ĳ���� �̵��ӵ� 
	}
}

void ACharacterBase::SetSprint(float WalkSpeed, float CameraSpeed)
{
	// ACharacterBase�� ī�޶� ���� ������ ������� �ʴ´�. 
	if (m_CharacterState == EBehaviorState::Idle || m_CharacterState == EBehaviorState::Battle)
	{
		m_CharacterState = EBehaviorState::Running;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// ĳ���� �̵��ӵ�
	}
}

void ACharacterBase::StopSprint()
{
	if (m_Mode == ECharacterMode::Idle)
	{
		m_CharacterState = EBehaviorState::Idle;
	}
	else if(m_Mode == ECharacterMode::Battle)
	{
		m_CharacterState = EBehaviorState::Battle;
	}

	GetCharacterMovement()->MaxWalkSpeed = m_OriginMoveSpeed;		// ĳ���� �̵��ӵ�
}
/// <summary>
/// ĳ���� �ܿ��� UseItem�� �Ѵ�.
/// </summary>
/// <param name="Index">������ ����Ʈ������ ��ȣ </param>
void ACharacterBase::UseItem(int32 Index)
{
	if (m_pInventoryComp == nullptr)
		return;

	// �ϴ� ������ int32�� �迭�� �� �ޱ� �ϴµ� �̰� �´���... 
	TArray<int32> ItemEffect = m_pInventoryComp->UseItem(Index);
	if (ItemEffect.IsEmpty() == false)
	{
		switch (ItemEffect[0])
		{
		case static_cast<int32>(EItemEffectType::PlusHP):
		{
			m_StatComp->AddHP(ItemEffect[2]);
			break;
		}
		case static_cast<int32>(EItemEffectType::PlusMP):
		{
			m_StatComp->AddMP(ItemEffect[2]);
			break;
		}
		case static_cast<int32>(EItemEffectType::PlusSP):
		{
			m_StatComp->AddSP(ItemEffect[2]);
			break;
		}
		case static_cast<int32>(EItemEffectType::PlusAttack):
		{
			FBaseStatusData Stat{};
			Stat.Attack = ItemEffect[2];
			m_StatComp->AddBaseStat(Stat);

			break;
		}
		case static_cast<int32>(EItemEffectType::PlusDefence):
		{
			FBaseStatusData Stat{};
			Stat.Defence = ItemEffect[2];
			m_StatComp->AddBaseStat(Stat);

			break;
		}

		default:
			break;
		}

	}

}
/// <summary>
/// ĳ���� �ܿ��� DropItem�� �Ѵ�.
/// </summary>
/// <param name="Index"></param>
/// <param name="Pos"></param>
void ACharacterBase::DropItem(int32 Index, FVector Pos)
{
	if (m_pInventoryComp == nullptr)
		return;
	m_pInventoryComp->DropItem(Index, Pos);
}

bool ACharacterBase::Equip(uint8 Part)
{
	// ��� �ƴѰ͵��� false��ȯ [ ���̷�Ż �޽ð� ������ ] 
	USkeletalMesh* SM = m_pInventoryComp->GetSkeletalMesh_Equipment(Part);
	if (SM != nullptr)
	{
		m_EquipmentMeshComp[Part]->SetSkeletalMesh(SM);
		return true;
	}
	return false;
}

bool ACharacterBase::UnEquip(uint8 Part)
{
	// ���߿� ��� ��ü �Ұ� ���¸� ���� ó���ϱ� ���� ��ȯ��.
	m_EquipmentMeshComp[Part]->SetSkeletalMesh(nullptr);
	return true;
}

void ACharacterBase::OnIdle()
{
}

void ACharacterBase::OnBattle()
{
}

void ACharacterBase::Attack()
{
}

void ACharacterBase::AttackCheck()
{
}

void ACharacterBase::Death()
{
}

void ACharacterBase::DeathEnd()
{
}

void ACharacterBase::OnHit()
{
}

void ACharacterBase::AttackEnd()
{
}

void ACharacterBase::OnDefense()
{
}

void ACharacterBase::StopDefense()
{
}

void ACharacterBase::Defense_Hit()
{
}

void ACharacterBase::OnDetect()
{
}

void ACharacterBase::OnAlert()
{
}

void ACharacterBase::OnCommand(uint8 Command)
{
}

void ACharacterBase::LookDirection(float Value)
{
	// ���� ������ �ִϸ��̼� ������ ������ ���� ������
	m_Horizontal = FMath::Clamp(Value, -1.f, 1.f);

	//Debug
	//UE_LOG(LogTemp, Log, TEXT("Horizontal : %f "), m_Horizontal);
}

void ACharacterBase::SetUpdateBehaviorDelegate(const FOnUpdateBehavior& Delegate)
{
}

bool ACharacterBase::IsOnTriggerEscape()
{
	return false;
}
