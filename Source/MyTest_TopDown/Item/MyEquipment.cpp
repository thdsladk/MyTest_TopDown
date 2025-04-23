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
	// 부착을 시킬수 없다 m_Trigger가 부모의 컴포넌트라서 ... 
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

			// 스켈레탈 메쉬 생성과정 [ 직접 경로 만들어서 붙여 주는중 ] 
			m_ItemInfo.EquipmentMesh = LoadObject<USkeletalMesh>(NULL, *ItemData->EquipmentMesh->GetPathName(), NULL, LOAD_None, NULL);

			// 스켈레탈 메쉬 생성과정 [ 객체를 직접 복사해서 붙여 주는중 ]
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
				m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.Scale);	// 스케일 두배로.
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
	// 땅에 떨어지면 줍기 , 아니면 다른 상호작용을 실행하도록 한다 .
// bool 보단 상태머신으로 만드는것도 좋을듯하다.

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
/// 안전하게 객체를 제거 또는 재활용 하기위한 함수
/// </summary>
/// <returns></returns>
bool AMyEquipment::RemoveObject()
{
	// 아이템 갯수가 0이 되어서 소멸을 호출 해야한다 .
	// 1. 소멸 시키기    2. 아이템 매니저를 만들어서 0이된 아이템은 매니저에서 보관 
	// (임시) 지금은 소멸 시키도록 해둔다.
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

