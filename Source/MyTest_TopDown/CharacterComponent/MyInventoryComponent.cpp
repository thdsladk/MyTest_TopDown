// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInventoryComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MyTest_TopDownGameMode.h"
#include "MyInventoryWidget.h"
#include "GameFramework/Character.h"

#include "Header/GlobalEnum.h"

//#include "MyItem.h"
#include "MyEquipment.h"




// Sets default values for this component's properties
UMyInventoryComponent::UMyInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	m_Inventory.Init(nullptr, m_ItemFirstIndex + (m_Row * m_Column));
	m_EquipmentMeshComp.Init(nullptr, m_ItemFirstIndex);

	m_PartTypeList.Init(TEXT(""), m_ItemFirstIndex);
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Head)] = TEXT("Head_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Hand_L)] = TEXT("LeftHand_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Chest)] = TEXT("Chest_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Hand_R)] = TEXT("RightHand_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Foot_L)] = TEXT("LeftFoot_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Foot_R)] = TEXT("RightFoot_Socket");

	// 장비 생성
	for (int32 Index = 0; Index < m_ItemFirstIndex; Index++)
	{
		// Equipment Component
		FString str = FString::Printf(TEXT("Equipment_%s"), *(m_PartTypeList[Index].ToString()));
		m_EquipmentMeshComp[Index] = CreateDefaultSubobject<USkeletalMeshComponent>(*str);
	}


}

void UMyInventoryComponent::BindMesh(USkeletalMeshComponent& Mesh)
{
	/*
	* 이 함수를 통해서 메시를 가져오는 이유는 Owner를 통해서 가져올 수도 있지만,
	* BeginPlay() 이후에 사용해야 할듯해서 그시점에 직접 BindMesh함수를 호출해서 
	가져오기 위함이다.    < 메시 안에 소캣들이 설정이 안되는 거같아서 시점을 뒤로 끌어서 해볼생각 >
	*/







}

void UMyInventoryComponent::InitializeComponent()
{
	// 인벤토리의 컨테이너는 여기서 초기화 !!!


	ACharacter* Owner = CastChecked<ACharacter>(GetOwner());
	USkeletalMeshComponent* CharacterMesh = Owner->GetMesh();
	//m_CharacterMesh = Owner->GetMesh();
	//if (m_CharacterMesh == nullptr) { return; }

	// 장비 초기화 
	for (int32 Index = 0; Index < m_ItemFirstIndex; Index++)
	{
		// Equipment Component
		
		if (Index == static_cast<int32>(EEquipmentPart::Hand_L) || Index == static_cast<int32>(EEquipmentPart::Hand_R))
		{
			if (CharacterMesh->DoesSocketExist(m_PartTypeList[Index]) == false)
			{
				UE_LOG(LogTemp, Log, TEXT("Not Found Socket Name!!!"));
			}
			m_EquipmentMeshComp[Index]->SetupAttachment(CharacterMesh, m_PartTypeList[Index]);
		}

/*
		// 제대로 부착되는지 로그 남기기.
		USkeletalMeshComponent* SM_Comp = CastChecked<USkeletalMeshComponent>(m_EquipmentMeshComp[Index]->GetAttachParent());
		if (SM_Comp == nullptr)
		{
			return;
		}

		if (SM_Comp == m_CharacterMesh.Get())
		{
			UE_LOG(LogTemp, Log, TEXT("Equipment successfully attached to character!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Attachment failed!"));
		}
*/


		// 현재는 Load하고 있는 장비창이 없지만, Load후에 초기 장착 세팅해주는 부분.
		/*
		AMyEquipment* Equipment = Cast<AMyEquipment>(m_Inventory[Index]);
		if (Equipment != nullptr)
		{
			m_EquipmentMeshComp[Index]->SetSkeletalMesh(Equipment->GetEquipmentMesh());
		}
		*/

	}



	// (legacy code 아이템 액터 부착 방식.)
	/*
	for (int32 Index = 0; Index < m_ItemFirstIndex; Index++)
	{

		if (m_Inventory[Index] != nullptr)
		{
			if(AttachEquipment(Index) == true)
			{
				// 장척 성공
				UE_LOG(LogTemp, Log, TEXT(" Attached : %s"), m_PartTypeList[Index]);
			}
			else
			{
				// 장착 실패
				UE_LOG(LogTemp, Log, TEXT(" Not Attached : %s"), m_PartTypeList[Index]);
			}
		}
		


	}
	*/

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
	// 강제로 특정 슬롯에 아이템을 밀어 넣을때 사용할 예정
	if (m_Inventory[Index] == nullptr)
	{
		m_Inventory[Index] = &Item;
		return true;
	}
	else
	{
		if (m_Inventory[Index]->GetCount() < Item.GetMaxCount()
			&& m_Inventory[Index]->GetID() == Item.GetID())
		{
			m_Inventory[Index] = &Item;
			return true;
		}

	}
	return false;

}

bool UMyInventoryComponent::TakeItem(AMyItem& Item)
{
	int32 Index = SearchPedingSlot(Item.GetID(), Item.GetCount());
	// 공간이 없을 때
	if (Index == -1) { return false; }


	if (m_Inventory[Index] == nullptr)
	{
		// 아이템 추가
		m_Inventory[Index] = &Item;
		return true;
	}
	else
	{
		// 아이템 병합
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
	if (m_Inventory[Index] != nullptr)
	{
		int32 ItemType = m_Inventory[Index]->UsingItem();
		int32 ItemTime = m_Inventory[Index]->GetEffectTime();
		int32 ItemIntensity = m_Inventory[Index]->GetEffectIntensity();


		if (ItemType != static_cast<int32>(EItemEffectType::None))	// None이면 Usable 아이템이 아니다.
		{
			PopItem(Index);
		}
		// Widget Update  : 여기서 전부 갱신해줄지 아니면 Index를 받아서 해당 해당 슬롯만 갱신해줄지 정하면 좋을듯
		//					임시로 인벤토리 위젯을 갱신한다.
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

bool UMyInventoryComponent::DropItem(int32 Index, FVector Pos)
{
	// 일단은 드롭하면 모든 갯수가 다 드롭되도록...
	if (m_Inventory[Index] != nullptr)
	{
		m_Inventory[Index]->Replace(Pos);

		// 지금은 그냥 떨어 트리지만 아이템 매니저를 만들면 매니저에게 넘겨야 할 수도 있다.
		m_Inventory[Index] = nullptr ;
		return true;
	}
	return false;
}

bool UMyInventoryComponent::MergetItems(AMyItem& Item, int32 To)
{
	// 인벤토리에 없는 아이템 + 인벤토리에 있는 아이템 일 경우 

	m_Inventory[To]->AddCount(Item.GetCount());

	if (Item.RemoveObject() == false)
	{
		// 삭제 실패 했을 경우.
		return false;
	}

	
	return true;
}

bool UMyInventoryComponent::MergeItems(int32 From, int32 To)
{
	// 인벤토리에 있는 아이템 끼리 병합 
	m_Inventory[To]->AddCount(m_Inventory[From]->GetCount());

	if (m_Inventory[From]->RemoveObject() == false)
	{
		//	삭제 실패 했을 경우.
		return false;
	}

	return true;
}

bool UMyInventoryComponent::SlitItems(int32 From, int32 To)
{
	// (보류) 몇대 몇으로 쪼개야할지 
	//			쪼개진 위치를 두곳을 어디를 지정할지 
	//			고민을 좀 해봐야할 것 같다. 


	return false;
}

void UMyInventoryComponent::SortInventory()
{
	// 아이템에 변동이 있을때 호출하도록 하자 .
	// ID번호가 더 작은 순서대로 정렬
	m_Inventory.Sort([](TWeakObjectPtr<AMyItem> A, TWeakObjectPtr <AMyItem> B) {return A->GetID() < B->GetID(); });
	
}

void UMyInventoryComponent::SwapInventory(int32 From, int32 To)
{
	// 리스트에서 객체를 세팅
	m_Inventory.Swap(From, To);

	// 위젯 작업 (각 객체의 위젯 Update호출 )
	m_SwapWidget.Execute(From, To);
}

bool UMyInventoryComponent::Equip(int32 FromInven, int32 ToEquip)
{
	AMyEquipment* Equipment = Cast<AMyEquipment>(m_Inventory[FromInven]);
	if (Equipment == nullptr)
	{
		// 장비가 아니다.
		return false;
	}
	else
	{
		// 장비가 맞다.
		if (m_Inventory[ToEquip] != nullptr)
		{
			AMyEquipment* PreEquipment = Cast<AMyEquipment>(m_Inventory[ToEquip]);
			// 장비가 장착 되어 있을 때.		// 교체한다.
			//DetachEquipment(ToEquip);

			AMyItem* Temp = m_Inventory[ToEquip].Get();
			m_Inventory[ToEquip] = Equipment;
			m_Inventory[FromInven] = Temp;


			//if (AttachEquipment(ToEquip) == true)
			{
				// 장착 성공
				Equipment->Equip();
				m_EquipmentMeshComp[ToEquip]->SetSkeletalMesh(Equipment->GetEquipmentMesh());
			}
		}
		else
		{
			// 장비가 장착 되어 있지 않을 때.
			m_Inventory[ToEquip] = Equipment;
			m_Inventory[FromInven] = nullptr;

			//if (AttachEquipment(ToEquip) == true)
			{
				// 장착 성공 
				m_EquipmentMeshComp[ToEquip]->SetSkeletalMesh(Equipment->GetEquipmentMesh());
				Equipment->Equip();
			}
		}
		// 위젯 갱신
		m_SwapWidget.Execute(FromInven, (ToEquip));

   		return true;
	}

}

bool UMyInventoryComponent::UnEquip(int32 FromEquip, int32 ToInven)
{
	for (int Index = m_ItemFirstIndex; Index < m_Inventory.Num(); Index++)
	{
		if (m_Inventory[Index] == nullptr)
		{
			m_Inventory[Index] = m_Inventory[FromEquip];
			m_Inventory[FromEquip] = nullptr;

			//DetachEquipment(FromEquip);
			
			m_EquipmentMeshComp[FromEquip]->SetSkeletalMesh(nullptr);

			// 위젯 갱신 
			m_SwapWidget.Execute(FromEquip, ToInven);
			
			return true;
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
		// 해당 인덱스가 장비 인덱스가 아니거나, 장비 칸에 장비가 아닌 무언가가 들어갔을 수 있다.
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
			// 해당 인덱스가 장비 인덱스가 아니거나, 장비 칸에 장비가 아닌 무언가가 들어갔을 수 있다.
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
			// 2. 첫 빈공간    
			// 아이템이 있다면 정렬되어서 nullptr이 안나온다. 없으면 nullptr부터 나온다.
			return Index;
		}
		else if (m_Inventory[Index]->GetID() != ID)
		{
			continue;
		}
		else if (m_Inventory[Index]->GetID() == ID)
		{
			// 1. ID가 일치하는 공간중에서 찾기 
			
			// 기존의 공간과 추가할 공간이 최대 공간을 넘지 않는지 확인
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

	// 여기까지오면 아이템이 들어갈 자리가 없다는 의미.
	return -1;
}
