// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInventoryComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MyTest_TopDownGameMode.h"
#include "MyInventoryWidget.h"

//#include "GameFramework/Character.h"
#include "Character/CharacterBase.h"

#include "Header/GlobalEnum.h"

#include "MyEquipment.h"




// Sets default values for this component's properties
UMyInventoryComponent::UMyInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	m_Inventory.Init(nullptr, m_ItemFirstIndex + (m_Row * m_Column));


	m_PartTypeList.Init(TEXT(""), m_ItemFirstIndex);
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Head)] = TEXT("Head_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Hand_L)] = TEXT("LeftHand_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Chest)] = TEXT("Chest_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Hand_R)] = TEXT("RightHand_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Foot_L)] = TEXT("LeftFoot_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Foot_R)] = TEXT("RightFoot_Socket");

	m_EquipmentMeshComp.Init(nullptr, m_ItemFirstIndex);


}

void UMyInventoryComponent::BindEquipment(TArray<USkeletalMeshComponent*> EquipmentList)
{
	// InitializeComponent ���Ŀ� ȣ�� �Ǿ�� ��ȿ�ϴ�. [ GetMesh�� ��ȿ�� �������� ] 


	for (int32 Index = 0; Index < m_ItemFirstIndex; Index++)
	{
		m_EquipmentMeshComp[Index] = EquipmentList[Index];
	}

	

}

void UMyInventoryComponent::InitializeComponent()
{
	// �κ��丮�� �����̳ʴ� ���⼭ �ʱ�ȭ !!!

	ACharacter* Owner = CastChecked<ACharacter>(GetOwner());
	USkeletalMeshComponent* CharacterMesh = Owner->GetMesh();
	if (CharacterMesh == nullptr)
	{
		return;
	}


}

UTexture2D* UMyInventoryComponent::GetThumbnail(int32 Index)
{
	if (m_Inventory[Index] == nullptr)
		return nullptr;
	return m_Inventory[Index]->GetThumbnail();
}

int32 UMyInventoryComponent::GetItemCount(int32 Index)
{
	if (m_Inventory[Index] == nullptr)
		return NULL;
	return m_Inventory[Index]->GetCount();
}

FString UMyInventoryComponent::GetItemName(int32 Index)
{
	if (m_Inventory[Index] == nullptr)
		return FString(TEXT("Empty"));
	return m_Inventory[Index]->GetName();
}

USkeletalMesh* UMyInventoryComponent::GetSkeletalMesh_Equipment(int32 Part)
{
	AMyEquipment* Equipment = Cast<AMyEquipment>(m_Inventory[Part]);
	if (Equipment != nullptr)
	{
		return Equipment->GetEquipmentMesh();
	}
	else
	{
		return nullptr;
	}
}

bool UMyInventoryComponent::TakeItem(AMyItem& Item, int32 Index)
{
	// ������ Ư�� ���Կ� �������� �о� ������ ����� ����
	if (m_Inventory[Index] == nullptr)
	{
		// ������ �ѱ��.
		Item.SetOwner(GetOwner());
		// ������ �߰�
		m_Inventory[Index] = &Item;
		return true;
	}
	else
	{
		if (m_Inventory[Index]->GetCount() + Item.GetCount() <= Item.GetMaxCount()
			&& m_Inventory[Index]->GetID() == Item.GetID())
		{
			// ������ ����
			return MergetItems(Item, Index);
			return true;
		}

	}
	return false;

}

bool UMyInventoryComponent::TakeItem(AMyItem& Item)
{
	int32 Index = SearchPedingSlot(Item.GetID(), Item.GetCount());
	// ������ ���� ��
	if (Index == -1) { return false; }


	if (m_Inventory[Index] == nullptr)
	{
		// ������ �ѱ��.
		Item.SetOwner(GetOwner());
		// ������ �߰�
		m_Inventory[Index] = &Item;
		return true;
	}
	else
	{
		// ������ ����
		return MergetItems(Item,Index);
	}

	return false;
}

AMyItem* UMyInventoryComponent::PopItem(int32 Index)
{
	return m_Inventory[Index].Get();
}

TArray<int32> UMyInventoryComponent::UseItem(int32 Index)
{
	AMyEquipment* Equipment = Cast<AMyEquipment>(m_Inventory[Index]);
	if (Equipment != nullptr)
	{
		Equip(Index);
		return TArray<int32>{};
	}
	else
	{
		if (m_Inventory[Index] != nullptr)
		{
			int32 ItemType = m_Inventory[Index]->UsingItem();
			int32 ItemTime = m_Inventory[Index]->GetEffectTime();
			int32 ItemIntensity = m_Inventory[Index]->GetEffectIntensity();


			if (ItemType != static_cast<int32>(EItemEffectType::None))	// None�̸� Usable �������� �ƴϴ�.
			{
				PopItem(Index);
			}
			// Widget Update  : ���⼭ ���� ���������� �ƴϸ� Index�� �޾Ƽ� �ش� �ش� ���Ը� ���������� ���ϸ� ������
			//					�ӽ÷� �κ��丮 ������ �����Ѵ�.
			//m_UpdateWidget.Broadcast();
			m_UpdateSlotWidget.Broadcast(Index);

			//ItemType
			return TArray<int32>{ItemType, ItemTime, ItemIntensity};

		}
		else
		{
			return TArray<int32>{};
		}
	}

	
}

bool UMyInventoryComponent::DropItem(int32 Index, FVector Pos)
{
	// �ϴ��� ����ϸ� ��� ������ �� ��ӵǵ���...
	if (m_Inventory[Index] != nullptr)
	{
		m_Inventory[Index]->Replace(Pos);

		// ������ �׳� ���� Ʈ������ ������ �Ŵ����� ����� �Ŵ������� �Ѱܾ� �� ���� �ִ�.
		m_Inventory[Index] = nullptr ;
		return true;
	}
	return false;
}

bool UMyInventoryComponent::MergetItems(AMyItem& Item, int32 To)
{
	// �κ��丮�� ���� ������ + �κ��丮�� �ִ� ������ �� ��� 

	m_Inventory[To]->AddCount(Item.GetCount());

	if (Item.RemoveObject() == false)
	{
		// ���� ���� ���� ���.
		return false;
	}

	
	return true;
}

bool UMyInventoryComponent::MergeItems(int32 From, int32 To)
{
	// �κ��丮�� �ִ� ������ ���� ���� 
	m_Inventory[To]->AddCount(m_Inventory[From]->GetCount());

	if (m_Inventory[From]->RemoveObject() == false)
	{
		//	���� ���� ���� ���.
		return false;
	}

	return true;
}

bool UMyInventoryComponent::SlitItems(int32 From, int32 To)
{
	// (����) ��� ������ �ɰ������� 
	//			�ɰ��� ��ġ�� �ΰ��� ��� �������� 
	//			����� �� �غ����� �� ����. 


	return false;
}

void UMyInventoryComponent::SortInventory()
{
	// �����ۿ� ������ ������ ȣ���ϵ��� ���� .
	// ID��ȣ�� �� ���� ������� ����
	m_Inventory.Sort([](TWeakObjectPtr<AMyItem> A, TWeakObjectPtr <AMyItem> B) {return A->GetID() < B->GetID(); });
	
}

void UMyInventoryComponent::SwapInventory(int32 From, int32 To)
{
	// ����Ʈ���� ��ü�� ����
	m_Inventory.Swap(From, To);

	// ���� �۾� (�� ��ü�� ���� Updateȣ�� )
	m_SwapWidget.Execute(From, To);
}

bool UMyInventoryComponent::Equip(int32 FromInven, int32 ToEquip)
{
	AMyEquipment* Equipment = Cast<AMyEquipment>(m_Inventory[FromInven]);
	if (Equipment == nullptr || Equipment->GetEquipmentPart() != static_cast<EEquipmentPart>(ToEquip))
	{
		// ��� �ƴϰų�, ���� ������ Ÿ�� ����ġ   ����
		return false;
	}
	else
	{ 
		if (m_Inventory[ToEquip] != nullptr)
		{
			AMyEquipment* PreEquipment = Cast<AMyEquipment>(m_Inventory[ToEquip]);
			// ��� ���� �Ǿ� ���� ��.		// ��ü�Ѵ�.
			PreEquipment->UnEquip();

			AMyItem* Temp = m_Inventory[ToEquip].Get();
			m_Inventory[ToEquip] = Equipment;
			m_Inventory[FromInven] = Temp;


			// ���� ����
			Equipment->Equip();

			// ĳ���Ϳ� ��� ���� 
			m_EquipmentMeshComp[ToEquip]->SetSkeletalMesh(Equipment->GetEquipmentMesh());

		}
		else
		{
			// ��� ���� �Ǿ� ���� ���� ��.
			m_Inventory[ToEquip] = Equipment;
			m_Inventory[FromInven] = nullptr;

			// ���� ���� 
			Equipment->Equip();

			// ĳ���Ϳ� ��� ���� 
			m_EquipmentMeshComp[ToEquip]->SetSkeletalMesh(Equipment->GetEquipmentMesh());


		}
		// ���� ����
		m_SwapWidget.Execute(FromInven, (ToEquip));

   		return true;
	}

}

bool UMyInventoryComponent::UnEquip(int32 FromEquip, int32 ToInven)
{
	if (m_Inventory[ToInven] == nullptr)
	{
		AMyEquipment* Equipment = Cast<AMyEquipment>(m_Inventory[FromEquip]);
		if (Equipment != nullptr)
		{
			m_Inventory[ToInven] = m_Inventory[FromEquip];
			m_Inventory[FromEquip] = nullptr;

			Equipment->UnEquip();

			m_EquipmentMeshComp[FromEquip]->SetSkeletalMesh(nullptr);

			// ���� ���� 
			m_SwapWidget.Execute(FromEquip, ToInven);

			return true;
		}

	}
	return false;

}
bool UMyInventoryComponent::Equip(int32 FromInven)
{
	AMyEquipment* Equipment = Cast<AMyEquipment>(m_Inventory[FromInven]);
	if (Equipment == nullptr)
	{
		// ��� �ƴ� ��   ����
		return false;
	}
	else
	{
		uint8 PartIndex = static_cast<int32>(Equipment->GetEquipmentPart());
		AMyEquipment* PreEquipment = Cast<AMyEquipment>(m_Inventory[PartIndex]);

		if (PreEquipment != nullptr)
		{
			// ��� ���� �Ǿ� ���� ��.		// ��ü�Ѵ�.
			PreEquipment->UnEquip();
			
			// ��ġ ��ȯ
			m_Inventory[PartIndex] = Equipment;
			m_Inventory[FromInven] = PreEquipment;

			// ���� ����
			Equipment->Equip();

			// ĳ���Ϳ� ��� ���� 
			m_EquipmentMeshComp[PartIndex]->SetSkeletalMesh(Equipment->GetEquipmentMesh());

		}
		else
		{
			// ��� ���� �Ǿ� ���� ���� ��.
			m_Inventory[PartIndex] = Equipment;
			m_Inventory[FromInven] = nullptr;

			// ���� ���� 
			Equipment->Equip();

			// ĳ���Ϳ� ��� ���� 
			m_EquipmentMeshComp[PartIndex]->SetSkeletalMesh(Equipment->GetEquipmentMesh());


		}
		// ���� ����
		m_SwapWidget.Execute(FromInven, PartIndex);

		return true;
	}
}
bool UMyInventoryComponent::UnEquip(int32 FromEquip)
{
	// (����) ���� ������ ���� �ʾƼ� SearchPedingSlot�� ������� �ʴ´�.
	for (int Index = m_ItemFirstIndex; Index < m_Inventory.Num(); Index++)
	{
		if (m_Inventory[Index] == nullptr)
		{
			AMyEquipment* Equipment = Cast<AMyEquipment>(m_Inventory[FromEquip]);
			if (Equipment != nullptr)
			{
				m_Inventory[Index] = m_Inventory[FromEquip];
				m_Inventory[FromEquip] = nullptr;

				Equipment->UnEquip();

				m_EquipmentMeshComp[FromEquip]->SetSkeletalMesh(nullptr);

				// ���� ���� 
				m_SwapWidget.Execute(FromEquip, Index);

				return true;
			}

		}
	}
	return false;
}
/*
bool UMyInventoryComponent::AttachEquipment(int32 Index)
{
	AMyEquipment* Equipment = Cast<AMyEquipment>(m_Inventory[Index]);
	if (Equipment != nullptr && m_CharacterMesh != nullptr)
	{
		if (Equipment->AttachToComponent(m_CharacterMesh.Get(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			m_PartTypeList[Index]) == false)
		{
			UE_LOG(LogTemp, Log, TEXT(" Not Attached : %s "), m_PartTypeList[Index]);
			return false;
		}
		else
		{
			return true;
		}

	}
	else
	{
		// �ش� �ε����� ��� �ε����� �ƴϰų�, ��� ĭ�� ��� �ƴ� ���𰡰� ���� �� �ִ�.
		return false;
	}


}

bool UMyInventoryComponent::DetachEquipment(int32 Index)
{
	AMyEquipment* Equipment = Cast<AMyEquipment>(m_Inventory[Index]);
		if (Equipment != nullptr)
		{
			Equipment->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			return true;
		}
		else
		{
			// �ش� �ε����� ��� �ε����� �ƴϰų�, ��� ĭ�� ��� �ƴ� ���𰡰� ���� �� �ִ�.
			return false;
		}

}
*/
int32 UMyInventoryComponent::SearchPedingSlot(int32 ID , int32 Amount)
{
	for (int Index = m_ItemFirstIndex; Index < m_Inventory.Num(); Index++)
	{
		if (m_Inventory[Index] == nullptr)
		{
			// 2. ù �����    
			// �������� �ִٸ� ���ĵǾ nullptr�� �ȳ��´�. ������ nullptr���� ���´�.
			return Index;
		}
		else if (m_Inventory[Index]->GetID() != ID)
		{
			continue;
		}
		else if (m_Inventory[Index]->GetID() == ID)
		{
			// 1. ID�� ��ġ�ϴ� �����߿��� ã�� 
			
			// ������ ������ �߰��� ������ �ִ� ������ ���� �ʴ��� Ȯ��
			if (m_Inventory[Index]->GetCount() + Amount <= m_Inventory[Index]->GetMaxCount())
			{
				return Index;
			}
			else
			{
				continue;
			}
		}
		
	}

	// ����������� �������� �� �ڸ��� ���ٴ� �ǹ�.
	return -1;
}
