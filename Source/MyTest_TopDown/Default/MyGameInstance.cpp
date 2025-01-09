// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{

    static ConstructorHelpers::FObjectFinder<UDataTable> DATA(TEXT("/Game/TopDown/Data/MyCharacterData.MyCharacterData"));
    if (DATA.Succeeded())
    {
        m_MyStats = DATA.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> MonsterDATA(TEXT("/Game/TopDown/Data/MonsterTable.MonsterTable"));
    if (MonsterDATA.Succeeded())
    {
        m_MonsterStats = MonsterDATA.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> ItemDATA(TEXT("/Game/TopDown/Data/ItemTable.ItemTable"));
    if (ItemDATA.Succeeded())
    {
        m_ItemTable = ItemDATA.Object;
    }
}


void UMyGameInstance::Init()
{
    Super::Init();

    if (GetStatData(1) != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("MyGameInstance %d"), GetStatData(1)->HP);

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Data is Empty "));
    }

}

FMyCharacterData* UMyGameInstance::GetStatData(int32 Level)
{
    FMyCharacterData* CharacterData = m_MyStats->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT(""));
    return CharacterData;
}

FMonsterData* UMyGameInstance::GetMonsterStatData(FString Name)
{
    FMonsterData* MonsterData = m_MonsterStats->FindRow<FMonsterData>(FName(Name), TEXT("Goblin"));
    return MonsterData;
}

FItemData* UMyGameInstance::GetItemData(int32 ID)
{
    FItemData* ItemData = m_ItemTable->FindRow<FItemData>(*FString::FromInt(ID), TEXT(""));
    return ItemData;
}
