// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInteractable.h"
#include "MyGameInstance.h"
#include "MyItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	NoneItem,
	ConsumableItem,
	Non_ConsumableItem,
	EquipmentItem,
	EtcItem,
	End
};


UCLASS()
class MYTEST_TOPDOWN_API AMyItem : public AMyInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyItem();
	
	virtual void SetItem(int32 ID);
public:
	FORCEINLINE int32 GetID() { return m_ItemInfo.ID; }
	FORCEINLINE int32 GetType() { return m_ItemInfo.Type; }
	FORCEINLINE FString GetName() { return m_ItemInfo.Name; }
	FORCEINLINE FString GetDesc() { return m_ItemInfo.Description; }
	FORCEINLINE int32 GetMaxCount() { return m_ItemInfo.MaxCount; }
	FORCEINLINE int32 GetScale() { return m_ItemInfo.Scale; }
	FORCEINLINE int32 GetEffectType() { return m_ItemInfo.EffectType; }
	FORCEINLINE int32 GetEffectTime() { return m_ItemInfo.EffectTime; }
	FORCEINLINE int32 GetEffectIntensity() { return m_ItemInfo.EffectIntensity; }
	FORCEINLINE UTexture2D* GetThumbnail() { return m_ItemInfo.Thumbnail;}

	virtual void Init()override;
	void Interact();
	virtual void Interact_Implementation();
	virtual void OnPickedUp();
	virtual int32 UsingItem()override;
	virtual void Replace(const FVector Pos)override;

	virtual void SetHidden(bool bHide)override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


	virtual void OnCharacterBeginOverlap(
			UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult
		)override;

	virtual void OnCharacterEndOverlap(
			UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;


protected:
	//// Item ���� ���� 
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//int32 m_ID;
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//int32 m_Type;
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//FString m_Name;
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//FString m_Desc;
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//int32 m_MaxCount;
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//int32 m_Scale;
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//int32 m_EffectType;
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//int32 m_EffectTime;
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//int32 m_EffectIntensity;
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//UTexture2D* m_Thumbnail;
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//UStaticMesh* m_ItemMesh;

	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	FItemData m_ItemInfo;


	// Item ���� ���� ( �������� �κ��丮�� ��â�� �迭�� �׿��־ �װ� �����̴�.
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//int32 m_Count;

	EItemType m_ItemType = EItemType::NoneItem;

	bool m_IsGround;
	


	



};
