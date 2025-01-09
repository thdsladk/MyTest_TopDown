// Fill out your copyright notice in the Description page of Project Settings.


#include "MyItem.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "ItemHelpTip.h"

#include "MyTest_TopDownCharacter.h"




// Sets default values
AMyItem::AMyItem()
	:m_ID(0) , m_IsGround(true) 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AMyItem::SetItem(int32 ID)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance != nullptr)
	{
		auto ItemData = MyGameInstance->GetItemData(ID);
		if (ItemData != nullptr)
		{
			m_ID = ItemData->ID;
			m_Type = ItemData->Type;
			m_Name = ItemData->Name;
			m_Desc = ItemData->Description;
			m_MaxCount = ItemData->MaxCount;
			m_Scale = ItemData->Scale;
			m_EffectType = ItemData->EffectType;
			m_EffectTime = ItemData->EffectTime;
			m_EffectIntensity = ItemData->EffectIntensity;

			m_Thumbnail = ItemData->Thumbnail;
			
			// ����ƽ �޽� �������� [ ���� ��� ���� �ٿ� �ִ��� ] 
			m_ItemMesh = LoadObject<UStaticMesh>(NULL, *ItemData->ItemMesh->GetPathName() , NULL, LOAD_None, NULL);
			// ����ƽ �޽� �������� [ ��ü�� ���� �����ؼ� �ٿ� �ִ��� ]
			//if (nullptr != ItemData->ItemMesh)
			//{
			//	DuplicateObject(ItemData->ItemMesh, m_ItemMesh);
			//}

			if (nullptr != m_ItemMesh)
			{
				m_MeshComp->SetStaticMesh(m_ItemMesh);
				//m_MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
				m_MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
				m_MeshComp->SetSimulatePhysics(true);
				m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_Scale);	// ������ �ι��.
			}
			// ���� �߰��ϱ� . [ �������� �� ���õ� ������ �ؾ��Ѵ�.
			m_InteractableHelpText = FString::Printf(TEXT("%s : Press F to Pick up "),*m_Name);

		}

	}
}

void AMyItem::Init()
{
	// �θ� ������ �ִ� ����� ����
	Super::Init();
	// �ڽ��� ������ �ִ� ����� ����


}

void AMyItem::Interact()
{
}

void AMyItem::Interact_Implementation()
{
	// ���� �������� �ݱ� , �ƴϸ� �ٸ� ��ȣ�ۿ��� �����ϵ��� �Ѵ� .
	// bool ���� ���¸ӽ����� ����°͵� �������ϴ�.

	if (m_IsGround) 
	{
		OnPickedUp();
	}
	else
	{
		Interact();

	}

}

void AMyItem::OnPickedUp()
{
	// OFF ���·� �����ֱ� . 

	m_IsGround = false;

	m_bVisiable = false;

	m_HelpTextComp->SetVisibility(m_bVisiable);
	//m_HelpTextComp->SetActive(false);

	SetHidden(true);

	m_Trigger->SetSimulatePhysics(false);
	m_Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

int32  AMyItem::UsingItem()
{
	return m_EffectType;
}

void AMyItem::Replace(const FVector Pos)
{
	// �θ� ������ �ִ� ����� ����
	Super::Replace(Pos);
	// �ڽ��� ������ �ִ� ����� ����
	m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_Scale);	// ������ �ι��.
	m_IsGround = true;
}

void AMyItem::SetHidden(bool bHide)
{
	m_MeshComp->SetVisibility(!bHide);
	m_MeshComp->SetSimulatePhysics(!bHide);
	m_MeshComp->SetHiddenInGame(bHide);
}

// Called when the game starts or when spawned
void AMyItem::BeginPlay()
{
	Super::BeginPlay();

	
}

void AMyItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetItem(m_ID);

	auto HelpWidget = Cast<UItemHelpTip>(m_HelpTextComp->GetUserWidgetObject());
	if (HelpWidget)
		HelpWidget->BindHelpTip(m_Name, m_InteractableHelpText);

}

void AMyItem::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCharacterBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AMyItem::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnCharacterEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
}


