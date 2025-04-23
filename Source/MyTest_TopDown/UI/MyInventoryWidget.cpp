// Fill out your copyright notice in the Description page of Project Settings.

#include "MyInventoryWidget.h"
#include "SlotWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Styling/SlateBrush.h"
#include "Engine/Texture2D.h"

#include "MyTest_TopDownCharacter.h"
#include "MyInventoryComponent.h"

#include "Header/GlobalEnum.h"


void UMyInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    m_EquipSlotList.Init(nullptr, 6U);
    m_ItemSlotList.Init(nullptr, m_Row * m_Column);
    
}

void UMyInventoryWidget::UpdateWidget()
{
    for (auto ItemSlot:m_ItemSlotList)
        ItemSlot->UpdateWidget();

    for (auto EquipmentSlot : m_EquipSlotList)
        EquipmentSlot->UpdateWidget();

}

void UMyInventoryWidget::UpdateWidget_Slot(int32 Index)
{
    if (Index < 0)
    {
        // ������ ������ ���� �� �ִ�.
    }
    else if(Index < static_cast<int32>(EEquipmentPart::End))
    {
        m_EquipSlotList[Index]->UpdateWidget();
    }
    else    // ���â ������ ������â
    {
        m_ItemSlotList[Index - static_cast<int32>(EEquipmentPart::End)]->UpdateWidget();
    }
}

void UMyInventoryWidget::BindInventory(UMyInventoryComponent& InventoryComp)
{
    // �κ��丮 ������Ʈ�� �޾Ƽ� ���⼭ ���ε�������.
    if (IsValid(&InventoryComp))
    {
        // ���ε� InventoryComponent �� InventoryWidget�� UpdateWidget�Լ��� ���ε�.
        InventoryComp.m_UpdateWidget.AddUObject(this, &UMyInventoryWidget::UpdateWidget);
        InventoryComp.m_UpdateSlotWidget.AddUObject(this, &UMyInventoryWidget::UpdateWidget_Slot);
        InventoryComp.m_SwapWidget.BindUObject(this, &UMyInventoryWidget::Swap_ItemSlot);
        
        // �ʱ� ���� 

        FString WidgetNamePattern = TEXT("EquipmentSlot_"); // �̹��� ���� �̸��� ����
        for (uint32 Index = 0U; Index < 6U; Index++)
        {
            // EquipSlot ������ �̸��� ����
            const FString EquipSlotWidgetName = WidgetNamePattern + FString::Printf(TEXT("%02d"), Index);

            // EquipSlot ������ ���ε�
            m_EquipSlotList[Index] = Cast<USlotWidget>(GetWidgetFromName(*EquipSlotWidgetName));

            // EquipSlot ���� �ʱ�ȭ
            m_EquipSlotList[Index]->BindInventory(&InventoryComp);

            // EquipSlot Setting        
            m_EquipSlotList[Index]->SetSlotIndex(Index);
            m_EquipSlotList[Index]->SetSlotType(ESlotType::EquipmentSlot);

            // ����
            m_EquipSlotList[Index]->UpdateWidget();
        }
        
        
        WidgetNamePattern = TEXT("ItemSlot_"); // �̹��� ���� �̸��� ����
        for (uint32 Index = 0U; Index < (m_Row * m_Column); Index++)
        {
            uint32 Index_Oct = 0U;
            Index_Oct += (Index / 8U) * 10U;
            Index_Oct += (Index % 8U);

            // ItemSlot ������ �̸��� ����
            const FString ItemSlotWidgetName = WidgetNamePattern + FString::Printf(TEXT("%02d"), Index_Oct);

            // ItemSlot ������ ���ε�
            m_ItemSlotList[Index] = Cast<USlotWidget>(GetWidgetFromName(*ItemSlotWidgetName));

            // ItemSlot ���� �ʱ�ȭ
            m_ItemSlotList[Index]->BindInventory(&InventoryComp);

            // ItemSlot Setting
            m_ItemSlotList[Index]->SetSlotIndex(Index +static_cast<int32>(EEquipmentPart::End));

            // ����
            m_ItemSlotList[Index]->UpdateWidget();
        }

    
    }
    
}






