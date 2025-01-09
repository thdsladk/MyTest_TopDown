// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	void Init();
	UFUNCTION()
	FEventReply MouseDown(FGeometry geo, const FPointerEvent& fevent);

	UFUNCTION()
	void UpdateWidget();
	UFUNCTION()
	void SetThumbnail(class UTexture2D* Texture);
	void SetTextBlock(class FText TextBlock);

	void SetItemIndex(int32 Index) { m_ItemIndex = Index; }
protected:

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemSlot;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemCount;

	int32 m_ItemIndex;

	UPROPERTY()
	TWeakObjectPtr<class UMyInventoryComponent> m_InventoryComp;


};
