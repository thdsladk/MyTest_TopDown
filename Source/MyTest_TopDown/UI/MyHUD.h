// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHUD.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkill, int8);

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UMyHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	~UMyHUD();
public:
	void Init();

	void BindHP(class UMyStatComponent* StatComp);
	void UpdateHP();
	void UpdateMP();
	void UpdateSP();

	void Update_HpRatioSync();

	void BindSkill(class USkillComponent* SkillComp);
	UFUNCTION()
	void UpdateBtnQ();
	UFUNCTION()
	void UpdateBtnW();
	UFUNCTION()
	void UpdateBtnE();
	UFUNCTION()
	void UpdateBtnR();

	UFUNCTION()
	void ClickInventory();

	void BindScreen1();
	void UpDateScreen1();

	FOnSkill m_OnSkill;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_HP;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_HP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_MP;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_MP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_SP;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_SP;


	// Button Widget Skill

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Skill1;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Skill2;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Skill3;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Skill4;


	// Button Widget Main/Sub

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Main;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Sub;

public:
	// Button Widget Inventory

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	class UButton* Btn_OpenChest;

private:
	// Button Widget Item

	UPROPERTY(meta = (BindWidget))
	class USlotWidget* Btn_Item1;
	UPROPERTY(meta = (BindWidget))
	class USlotWidget* Btn_Item2;
	UPROPERTY(meta = (BindWidget))
	class USlotWidget* Btn_Item3;
	UPROPERTY(meta = (BindWidget))
	class USlotWidget* Btn_Item4;
	UPROPERTY(meta = (BindWidget))
	class USlotWidget* Btn_Item5;
	UPROPERTY(meta = (BindWidget))
	class USlotWidget* Btn_Item6;


	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Screen1;


	TWeakObjectPtr<UMyStatComponent> m_CurrentStatComp;
	//TWeakObjectPtr<USkillComponent> m_SkillComp;	// HUD가 스킬 컴포넌트를 들고있을 이유가 없다.


	// Timer 
	FTimerHandle m_Timer_Screen1;

	FTimerHandle m_HpSync;

private:		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_HpbarRatio = 1.f;

	
};
