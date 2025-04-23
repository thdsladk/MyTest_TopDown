// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "MyInventoryComponent.generated.h"

// define 
#define EmptyIndex 100


DECLARE_MULTICAST_DELEGATE(FOnUpdateWidget);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateSlotWidget,int32);

// �ϳ��� �Լ��� ���ε� �ϱ����Ͽ�...
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

	// ������ ���� �κ� 
	AMyItem* PopItem(int32 Index);

	bool TakeItem(AMyItem& Item, int32 Index);
	bool TakeItem(AMyItem& Item);

	TArray<int32> UseItem(int32 Index);
	bool DropItem(int32 Index,FVector Pos);

	// ������ ����,�и�
	bool MergetItems(AMyItem& Item, int32 To);
	bool MergeItems(int32 From,int32 To);
	bool SlitItems(int32 From, int32 To);

	void SortInventory();

	// ������ �̵� ���� �Լ�
	void SwapInventory(int32 From, int32 To);


	// Equipment Section
	bool Equip(int32 FromInven , int32 ToEquip);
	bool UnEquip(int32 FromEquip, int32 ToInven);
	

	FORCEINLINE uint32 GetInventorySize() { return (m_Row * m_Column); }
	FORCEINLINE uint32 GetTotalSize() { return m_Inventory.Num(); }

protected:
	// Equipment ( ���ο� ) 
	/*
	bool AttachEquipment(int32 Index);
	bool DetachEquipment(int32 Index);
	*/
private:
	// ���� ���� �Լ��� 
	int32 SearchPedingSlot(int32 ID, int32 Amount = 1);


public:
	FOnUpdateWidget m_UpdateWidget;				// �κ��丮 �����̶� ���ε�����
	FOnUpdateSlotWidget m_UpdateSlotWidget;		
	FOnSwapWidget m_SwapWidget;		// ��ȯ ���� ��������Ʈ[ �̱� ��������Ʈ ] 
protected:
	// �����۵��� ��������... ��� �δ°��� ������ ... �κ��丮�� ������ �ϴ°� ?
	TArray<TWeakObjectPtr<AMyItem>>			m_Inventory;
	/*
	* ������ ����Ʈ + ��� ����Ʈ ������ �κ��丮�� �����Ǿ��ִ�. 
	*/
	TWeakObjectPtr<USkeletalMeshComponent> m_CharacterMesh;
	
	const int32 m_ItemFirstIndex = 6;

	TArray<FName> m_PartTypeList;

	// ��� �޽� ������Ʈ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TArray<TObjectPtr<USkeletalMeshComponent>> m_EquipmentMeshComp;




	const uint32 m_Row = 5U;
	const uint32 m_Column = 8U;
};
