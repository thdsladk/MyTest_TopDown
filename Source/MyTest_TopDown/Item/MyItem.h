// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyInteractable.h"
#include "Header/DataStruct.h"
#include "MyItem.generated.h"


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
	FORCEINLINE UStaticMesh* GetItemMesh() { return m_ItemInfo.ItemMesh; }
	FORCEINLINE USkeletalMesh* GetEquipmentMesh() { return m_ItemInfo.EquipmentMesh; }



	FORCEINLINE const int32 GetCount() { return m_Count; }
	void AddCount(int32 Count);


	virtual void Init()override;
	void OnInteract();
	virtual void Interact_Implementation()override;
	virtual void OnPickedUp();
	virtual int32 UsingItem()override;
	virtual void Replace(const FVector Pos)override;

	virtual void SetHidden(bool bHide)override;

	virtual bool RemoveObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


	//virtual void OnCharacterBeginOverlap(
	//		UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	//		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	//		bool bFromSweep, const FHitResult& SweepResult
	//	)override;
	//
	//virtual void OnCharacterEndOverlap(
	//		UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	//		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;


protected:

	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	FItemData m_ItemInfo;

	bool m_IsGround;
	
	// 유동 값 [ 게이지류 ] 
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	int32 m_Count;


	


	



};
