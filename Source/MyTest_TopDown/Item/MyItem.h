// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInteractable.h"
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
	int32 GetID() { return m_ID; }
	int32 GetType() { return m_Type; }
	FString GetName() { return m_Name; }
	FString GetDesc() { return m_Desc; }
	int32 GetMaxCount() { return m_MaxCount; }
	int32 GetScale() { return m_Scale; }
	int32 GetEffectType() { return m_EffectType; }
	int32 GetEffectTime() { return m_EffectTime; }
	int32 GetEffectIntensity() { return m_EffectIntensity; }
	UTexture2D* GetThumbnail() { return m_Thumbnail;}

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

public:



protected:
	// Item 고정 정보 
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	int32 m_ID;
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	int32 m_Type;
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	FString m_Name;
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	FString m_Desc;
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	int32 m_MaxCount;
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	int32 m_Scale;
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	int32 m_EffectType;
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	int32 m_EffectTime;
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	int32 m_EffectIntensity;
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	UTexture2D* m_Thumbnail;
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	UStaticMesh* m_ItemMesh;

	// Item 변동 정보 ( 아이템은 인벤토리의 각창에 배열로 쌓여있어서 그게 갯수이다.
	//UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	//int32 m_Count;

	EItemType m_ItemType = EItemType::NoneItem;

	bool m_IsGround;
	


	



};
