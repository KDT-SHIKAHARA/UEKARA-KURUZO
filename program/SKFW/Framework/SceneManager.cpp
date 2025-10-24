
#include<SKFW/Framework/SceneManager.hpp>
#include<SKFW/Framework/Camera.h>


#include<SKFW/Ecs/GameObjectManager.hpp>
#include<SKFW/Ecs/system_RigidBody.hpp>
#include<SKFW/Ecs/system_Collision.hpp>
#include<SKFW/Ecs/system_Render.hpp>

#include<SKFW/Framework/DefaultScene.hpp>

void SceneManager::Initialize()
{
	if (m_scene == nullptr)
	{
		this->CreateScene<DefaultScene>();
	}
	else
	{
		m_scene->Initialize();
	}

}

/// <summary>
/// 可変更新
/// </summary>
/// <param name="dt"></param>
void SceneManager::Update(float a_dt)
{
	//	スクリーンがないときは処理を行わない
	if (m_scene == nullptr)
	{
		return;
	}

	//	オーバーレイが存在しているとき
	if (m_overlay != nullptr)
	{
		m_overlay->Update(a_dt);
		
		//	終了フラグが立っていたら削除
		if (m_overlay->m_isFinish == true)
		{
			m_overlay.reset();
		}
	}
	else
	{
		//	ここに条件式を追加してオーバーレイがあるときは行わないようにする。
		GameObjectManager::Instance().Update(a_dt);
	}


}

/// <summary>
/// 固定更新
/// </summary>
/// <param name="a_fixedDt"></param>
void SceneManager::FixedUpdate(float a_fixedDt)
{
	//	スクリーンがないときは処理を行わない
	if (m_scene == nullptr)
	{
		return;
	}

	if(m_overlay == nullptr)
	{
		//	ここの条件式を追加してオーバーレイがあるときは行わないようにする。
		GameObjectManager::Instance().FixedUpdate(a_fixedDt);
		RigidBodySystem::Instance().FixedUpdate(a_fixedDt);
		CollisionSystem::Instance().FixedUpdate(a_fixedDt);
	}
}

void SceneManager::Render(float a_alpha, sf::RenderWindow& a_window)
{
	//	既存の描画処理を書く
		//	ワールド
	Camera::Instance().ApplyWorldView(a_window);
	RenderSystem::Instance().WorldObjectRender(a_window, a_alpha);

	//	スクリーン
	Camera::Instance().ApplySceneView(a_window);
	RenderSystem::Instance().SceneObjectRender(a_window, a_alpha);

	//	トランジションのがあるときだけ描画
	if (m_overlay != nullptr)
	{
		m_overlay->Render(a_window);
	}

}

/// <summary>
/// スクリーンの切り替え判定と切り替え
/// </summary>
void SceneManager::ChangeScene()
{
	if (m_pendingChange != nullptr)
	{
		m_pendingChange();
		m_pendingChange = nullptr;
	}
}
