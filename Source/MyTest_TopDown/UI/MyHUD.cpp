// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "MyStatComponent.h"
#include "SkillComponent.h"
#include "MyTest_TopDownPlayerController.h"
#include "SlotWidget.h"

UMyHUD::~UMyHUD()
{
	//GetWorld()->GetTimerManager().ClearTimer(m_Timer_Screen1);
	
}

void UMyHUD::NativeDestruct()
{
	GetWorld()->GetTimerManager().ClearTimer(m_HpSync);
}

void UMyHUD::Init()
{
	Btn_Item1->Init();
	Btn_Item2->Init();
	Btn_Item3->Init();
	Btn_Item4->Init();
	Btn_Item5->Init();
	Btn_Item6->Init();

	GetWorld()->GetTimerManager().SetTimer(m_HpSync, this, &UMyHUD::Update_HpRatioSync, 0.1f, true);
}

void UMyHUD::BindHP(UMyStatComponent* StatComp)
{
	m_StatComp = StatComp;
	StatComp->OnHPChanged.AddUObject(this, &UMyHUD::UpdateHP);
	StatComp->OnMPChanged.AddUObject(this, &UMyHUD::UpdateMP);
	StatComp->OnSPChanged.AddUObject(this, &UMyHUD::UpdateSP);

	m_PreHpRatio = m_StatComp->GetHPRatio();

	// Screen1 Bind 
	StatComp->OnHPChanged.AddUObject(this, &UMyHUD::UpDateScreen1);



}
void UMyHUD::UpdateHP()
{
	if (m_StatComp.IsValid())
	{
		if (m_HpSync.IsValid())
		{
			m_CurrentHpRatio = m_StatComp->GetHPRatio();

		}
	}
}

void UMyHUD::UpdateMP()
{
	if (m_StatComp.IsValid())
	{
		m_CurrentMpRatio = m_StatComp->GetMPRatio();
		ProgressBar_MP->SetPercent(m_CurrentMpRatio);
	}
}

void UMyHUD::UpdateSP()
{
	if (m_StatComp.IsValid())
	{
		m_CurrentSpRatio = m_StatComp->GetSPRatio();
		ProgressBar_SP->SetPercent(m_CurrentSpRatio);
	}
}

void UMyHUD::Update_HpRatioSync()
{
	if (FMath::IsNearlyEqual(m_CurrentHpRatio, m_PreHpRatio, KINDA_SMALL_NUMBER) == false)
	{
		m_PreHpRatio = FMath::Clamp<float>(m_PreHpRatio + (m_CurrentHpRatio - m_PreHpRatio) * 0.2f, m_CurrentHpRatio, m_PreHpRatio);
		ProgressBar_HP->SetPercent(m_PreHpRatio);
	}
	else
	{
		UE_LOG(LogTemp,Log,TEXT(" Sync Current Hp == Pre Hp "))
	}
}

void UMyHUD::BindSkill(USkillComponent* SkillComp)
{
	Btn_Skill1->OnClicked.AddDynamic(this, &UMyHUD::UpdateBtnQ);
	Btn_Skill2->OnClicked.AddDynamic(this, &UMyHUD::UpdateBtnW);
	Btn_Skill3->OnClicked.AddDynamic(this, &UMyHUD::UpdateBtnE);
	Btn_Skill4->OnClicked.AddDynamic(this, &UMyHUD::UpdateBtnR);

	Btn_OpenChest->OnClicked.AddDynamic(this, &UMyHUD::ClickInventory);

	m_OnSkill.AddUObject(SkillComp, &USkillComponent::Click_Btn);


}

void UMyHUD::UpdateBtnQ()
{
	m_OnSkill.Broadcast(USkillComponent::ESkill::Skill_Q);
	//m_SkillComp->Click_Q();
}

void UMyHUD::UpdateBtnW()
{
	m_OnSkill.Broadcast(USkillComponent::ESkill::Skill_W);
	//m_SkillComp->Click_W();
}

void UMyHUD::UpdateBtnE()
{
	m_OnSkill.Broadcast(USkillComponent::ESkill::Skill_E);
	//m_SkillComp->Click_E();
}

void UMyHUD::UpdateBtnR()
{
	m_OnSkill.Broadcast(USkillComponent::ESkill::Skill_R);
	//m_SkillComp->Click_R();
}

void UMyHUD::ClickInventory()
{
	Cast<AMyTest_TopDownPlayerController>(GetWorld()->GetFirstPlayerController())->Inventory_Notify.Broadcast();
}

void UMyHUD::BindScreen1()
{
	Img_Screen1->SetVisibility(ESlateVisibility::Hidden);

	GetWorld()->GetTimerManager().SetTimer(m_Timer_Screen1, this, &UMyHUD::UpDateScreen1, 1.f, true, 0.f);

}

void UMyHUD::UpDateScreen1()
{
	if (m_StatComp.IsValid())
	{
		float RatioHP = m_StatComp.Get()->GetHPRatio();

		if (
			RatioHP < 0.5f && 
			Img_Screen1->GetVisibility()== ESlateVisibility::Hidden)
		{
			
			Img_Screen1->SetVisibility(ESlateVisibility::Visible);

		}
		else if(
			RatioHP >= 0.5f && 
			Img_Screen1->GetVisibility() == ESlateVisibility::Visible)
		{
			Img_Screen1->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		FLinearColor LinearColor{ 0.f,0.f,0.f,0.05f };
		if (Img_Screen1->GetColorAndOpacity().A >= 0.05f)
		{
			Img_Screen1->SetColorAndOpacity(Img_Screen1->GetColorAndOpacity() - LinearColor);

		}
		else
		{
			LinearColor.A = 0.2f;
			Img_Screen1->SetColorAndOpacity(Img_Screen1->GetColorAndOpacity() + LinearColor);

		}


	}

}


