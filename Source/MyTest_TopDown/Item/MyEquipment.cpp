// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEquipment.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "EditorFramework/AssetImportData.h"
#include "Components/WidgetComponent.h"

#include "ItemHelpTip.h"


// Sets default values
AMyEquipment::AMyEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_EquipmentMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
	// ������ ��ų�� ���� m_Trigger�� �θ��� ������Ʈ�� ... 
	//m_EquipmentMeshComp->SetupAttachment(m_Trigger.Get());
}

void AMyEquipment::SetEquipment(int32 ID)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance != nullptr)
	{
		auto ItemData = MyGameInstance->GetItemData(ID);
		if (ItemData != nullptr)
		{

			// ���̷�Ż �޽� �������� [ ���� ��� ���� �ٿ� �ִ��� ] 
			m_ItemInfo.EquipmentMesh = LoadObject<USkeletalMesh>(NULL, *ItemData->EquipmentMesh->GetPathName(), NULL, LOAD_None, NULL);

			// ���̷�Ż �޽� �������� [ ��ü�� ���� �����ؼ� �ٿ� �ִ��� ]
			//if (nullptr != ItemData->ItemMesh)
			//{
			//	DuplicateObject(ItemData->ItemMesh, m_ItemMesh);
			//}

			if (nullptr != m_ItemInfo.EquipmentMesh)
			{
				m_EquipmentMeshComp->SetSkeletalMesh(m_ItemInfo.EquipmentMesh);
				//m_MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
				m_MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
				m_MeshComp->SetSimulatePhysics(true);
				m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.Scale);	// ������ �ι��.
			}
			
		}
	}

}

void AMyEquipment::SetItem(int32 ID)
{
	Super::SetItem(ID);

	SetEquipment(ID);

}

void AMyEquipment::Equip()
{
	if (m_IsEquip == false)
	{
		m_IsEquip = true;
		//SwitchVisiable();
		m_bVisiable = true;
		SetHidden(!m_bVisiable);
	}

}

void AMyEquipment::UnEquip()
{
	if (m_IsEquip == true)
	{
		m_IsEquip = false;
		//SwitchVisiable();
		m_bVisiable = false;
		SetHidden(!m_bVisiable);
	}
}

void AMyEquipment::OnInteract()
{
}

void AMyEquipment::Interact_Implementation()
{
	// ���� �������� �ݱ� , �ƴϸ� �ٸ� ��ȣ�ۿ��� �����ϵ��� �Ѵ� .
// bool ���� ���¸ӽ����� ����°͵� �������ϴ�.

	if (m_IsGround)
	{
		OnPickedUp();
	}
	else
	{
		OnInteract();
	}
}

void AMyEquipment::OnPickedUp()
{
	Super::OnPickedUp();
}
/// <summary>
/// �����ϰ� ��ü�� ���� �Ǵ� ��Ȱ�� �ϱ����� �Լ�
/// </summary>
/// <returns></returns>
bool AMyEquipment::RemoveObject()
{
	// ������ ������ 0�� �Ǿ �Ҹ��� ȣ�� �ؾ��Ѵ� .
	// 1. �Ҹ� ��Ű��    2. ������ �Ŵ����� ���� 0�̵� �������� �Ŵ������� ���� 
	// (�ӽ�) ������ �Ҹ� ��Ű���� �صд�.
	// return   IsPendingKillPending 

	return Destroy();
}

// Called when the game starts or when spawned
void AMyEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

//void AMyEquipment::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	AMyItem::OnCharacterBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
//}
//
//void AMyEquipment::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	AMyItem::OnCharacterEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
//}

