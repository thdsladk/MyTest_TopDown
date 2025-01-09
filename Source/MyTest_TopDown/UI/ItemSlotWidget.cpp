// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


#include "MyTest_TopDownCharacter.h"
#include "MyInventoryComponent.h"

void UItemSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ItemSlot->OnMouseButtonDownEvent.BindDynamic(this, &UItemSlotWidget::MouseDown);
}

void UItemSlotWidget::Init()
{
    AMyTest_TopDownCharacter* MyCharacter = Cast<AMyTest_TopDownCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (MyCharacter != nullptr && MyCharacter->GetInventory() != nullptr)
    {
        m_InventoryComp = MyCharacter->GetInventory();
    }
}

FEventReply UItemSlotWidget::MouseDown(FGeometry geo, const FPointerEvent& fevent)
{
    AMyTest_TopDownCharacter* MyCharacter = Cast<AMyTest_TopDownCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (MyCharacter != nullptr )
    {
        MyCharacter->UseItem(m_ItemIndex);
    }

    return FEventReply(false);

}


void UItemSlotWidget::UpdateWidget()
{
    // 썸네일 세팅 
    ItemSlot->SetBrushFromTexture(m_InventoryComp->GetThumbnail(m_ItemIndex));
     
    // 아이템 갯수 세팅   
    ItemCount->SetText(FText::FromString(FString::FromInt(m_InventoryComp->GetItemCount(m_ItemIndex))));

}

void UItemSlotWidget::SetThumbnail(class UTexture2D* Texture)
{
    if (Texture != nullptr)
    {
        ItemSlot->SetBrushFromTexture(Texture);
    }

}

void UItemSlotWidget::SetTextBlock(FText TextBlock)
{
    ItemCount->SetText(TextBlock);
}
