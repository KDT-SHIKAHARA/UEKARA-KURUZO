#include<SKFW/Framework/Scene.hpp>
#include<SKFW/Ecs/GameObjectManager.hpp>

Scene::Scene(const std::string& a_sceneName)
	:m_name(a_sceneName)
{
}

void Scene::Finalize()
{
	//	GameObject�̃C���X�^���X��j������B
	GameObjectManager::Instance().AllClear();
}

void Scene::LoadResources()
{
	//	����͌l�p�ŊJ�n���Ɏ����Ǎ��ɂ��Ă��܂��B
	//	���\�[�X���������Ă������Ȃ�X�N���[�����̃f�B���N�g����
	//	���\�[�X�������Ă��̃f�B���N�g���z���̃��\�[�X��ǂݍ��ޕ�����
	//	���ꂪ�s�ւ����Ȃ烌�W�X�g���t�@�C��������Č���������E
}

void Scene::UnloadResources()
{
}

std::string Scene::GetName() const noexcept
{
	return m_name;
}

