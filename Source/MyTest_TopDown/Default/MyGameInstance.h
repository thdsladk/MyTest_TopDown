// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */

USTRUCT()
struct FMyCharacterData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxMP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAttack;

	FMyCharacterData operator+(const FMyCharacterData& Other) const
	{
		const int32* const ThisPtr = reinterpret_cast<const	 int32* const>(this);
		const int32* const OtherPtr = reinterpret_cast<const int32* const>(&Other);

		FMyCharacterData Result;
		int32* ResultPtr = reinterpret_cast<int32*>(&Result);
		int32 StatNum = sizeof(FMyCharacterData) / sizeof(int32);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};

USTRUCT()
struct FMonsterData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxMP;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxDefence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* LEquipMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* REquipMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RItemID;

};

USTRUCT()
struct FItemData : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EffectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EffectTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EffectIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ItemMesh;
	
};

UENUM()
enum EItemEffectType : int32
{
	None,
	PlusHP,
	PlusMP,
	PlusSP,
	PlusAttack,
	PlusDefence,
	End

};

UCLASS()
class MYTEST_TOPDOWN_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UMyGameInstance();

	virtual void Init() override;

	FMyCharacterData* GetStatData(int32 Level);
	FMonsterData* GetMonsterStatData(FString Name);
	FItemData* GetItemData(int32 ID);

private:

	UPROPERTY()
	class UDataTable* m_MyStats;

	UPROPERTY()
	class UDataTable* m_MonsterStats;

	UPROPERTY()
	class UDataTable* m_ItemTable;


	

};
