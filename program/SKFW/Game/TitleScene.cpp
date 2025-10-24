#include "TitleScene.h"

#include<SKFW/Ecs/Ecs.h>

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
	auto& obj = GameObjectManager::Instance().Create(eObjectLivedType::Long, "None");
	obj->AddComponent<TransformComponent>();
	obj->AddComponent<ImageComponent>(TextureResourceManager::Instance().GetTexture("assets/textures/title/bg.jpeg"),0);

	//	スタートボタンの生成

	//	
}
