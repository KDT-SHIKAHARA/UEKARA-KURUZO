#include "TitleScene.h"

#include<SKFW/Ecs/Ecs.h>

#include"comp_titleStartButton.h"

TitleScene::TitleScene()
	:Scene("Title Scene")
{
}

/// <summary>
/// インスタンスの生成をする
/// </summary>
void TitleScene::Initialize()
{
	//	背景の生成
	auto& bg = GameObjectManager::Instance().Create(eObjectLivedType::Long, "None");
	auto tnras = bg->AddComponent<TransformComponent>();
	tnras->SetPivot(TransformComponent::ePivot::TopLeft);
	bg->AddComponent<ImageComponent>(TextureResourceManager::Instance().GetTexture("assets/textures/title/bg.jpeg"),0);

	//	スタートボタンの生成
	auto& start = GameObjectManager::Instance().Create(eObjectLivedType::Long, "None");
	start->AddComponent<TitleButton>();
	auto trans = start->AddComponent<TransformComponent>();
	trans->SetPosition(static_cast<sf::Vector2f>(WindowData::m_video.size) / 2.0f);
	start->AddComponent<ImageComponent>(TextureResourceManager::Instance().GetTexture("assets/textures/title/guide.png"), 1);
	//	
}
