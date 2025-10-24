#include<SKFW/Framework/Scene.hpp>
#include<SKFW/Ecs/GameObjectManager.hpp>

Scene::Scene(const std::string& a_sceneName)
	:m_name(a_sceneName)
{
}

void Scene::Finalize()
{
	//	GameObjectのインスタンスを破棄する。
	GameObjectManager::Instance().AllClear();
}

void Scene::LoadResources()
{
	//	現状は個人用で開始時に自動読込にしています。
	//	リソースが圧迫していそうならスクリーン名のディレクトリに
	//	リソースをおいてそのディレクトリ配下のリソースを読み込む方式で
	//	それが不便そうならレジストリファイルを作って効率化する・
}

void Scene::UnloadResources()
{
}

std::string Scene::GetName() const noexcept
{
	return m_name;
}

