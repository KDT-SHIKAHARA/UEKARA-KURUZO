#include "TitleScene.h"

#include<SKFW/Ecs/Ecs.h>

TitleScene::TitleScene()
	:Scene("Title Scene")
{
}

/// <summary>
/// �C���X�^���X�̐���������
/// </summary>
void TitleScene::Initialize()
{
	//	�w�i�̐���
	auto& obj = GameObjectManager::Instance().Create(eObjectLivedType::Long, "None");
	obj->AddComponent<TransformComponent>();
	obj->AddComponent<ImageComponent>(TextureResourceManager::Instance().GetTexture("assets/textures/title/bg.jpeg"),0);

	//	�X�^�[�g�{�^���̐���

	//	
}
