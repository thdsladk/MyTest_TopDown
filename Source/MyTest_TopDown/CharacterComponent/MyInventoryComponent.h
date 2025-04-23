// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "MyInventoryComponent.generated.h"

// define 
#define EmptyIndex 100


DECLARE_MULTICAST_DELEGATE(FOnUpdateWidget);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateSlotWidget,int32);

// 하나의 함수만 바인딩 하기위하여...
DECLARE_DELEGATE_TwoParams(FOnSwapWidget,int32,int32);

class AMyItem;
class AMyEquipment;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API UMyInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyInventoryComponent();
	void BindMesh(USkeletalMeshComponent& Mesh);

protected:
	virtual void InitializeComponent() override;

public:	
	UTexture2D* GetThumbnail(int32 Index); 
	int32 GetItemCount(int32 Index);
	FString GetItemName(int32 Index);

	USkeletalMesh* GetSkeletalMesh_Equipment(int32 Part);

	// 아이템 조작 부분 
	AMyItem* PopItem(int32 Index);

	bool TakeItem(AMyItem& Item, int32 Index);
	bool TakeItem(AMyItem& Item);

	TArray<int32> UseItem(int32 Index);
	bool DropItem(int32 Index,FVector Pos);

	// 아이템 병합,분리
	bool MergetItems(AMyItem& Item, int32 To);
	bool MergeItems(int32 From,int32 To);
	bool SlitItems(int32 From, int32 To);

	void SortInventory();

	// 아이템 이동 관련 함수
	void SwapInventory(int32 From, int32 To);


	// Equipment Section
	bool Equip(int32 FromInven , int32 ToEquip);
	bool UnEquip(int32 FromEquip, int32 ToInven);
	

	FORCEINLINE uint32 GetInventorySize() { return (m_Row * m_Column); }
	FORCEINLINE uint32 GetTotalSize() { return m_Inventory.Num(); }

protected:
	// Equipment ( 내부용 ) 
	/*
	bool AttachEquipment(int32 Index);
	bool DetachEquipment(int32 Index);
	*/
private:
	// 내부 계산용 함수들 
	int32 SearchPedingSlot(int32 ID, int32 Amount = 1);


public:
	FOnUpdateWidget m_UpdateWidget;				// 인벤토리 위젯이랑 바인딩하자
	FOnUpdateSlotWidget m_UpdateSlotWidget;		
	FOnSwapWidget m_SwapWidget;		// 교환 전용 델리게이트[ 싱글 델리게이트 ] 
protected:
	// 아이템들의 소유권은... 어디에 두는것이 옳은가 ... 인벤토리는 참조만 하는가 ?
	TArray<TWeakObjectPtr<AMyItem>>			m_Inventory;
	/*
	* 아이템 리스트 + 장비 리스트 순으로 인벤토리는 구성되어있다. 
	*/
	TWeakObjectPtr<USkeletalMeshComponent> m_CharacterMesh;
	
	const int32 m_ItemFirstIndex = 6;

	TArray<FName> m_PartTypeList;

	// 장비 메시 컴포넌트 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TArray<TObjectPtr<USkeletalMeshComponent>> m_EquipmentMeshComp;




	const uint32 m_Row = 5U;
	const uint32 m_Column = 8U;
};
