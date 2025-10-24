#include<SKFW/Ecs/system_Render.hpp>

#include<SKFW/Ecs/GameObject.hpp>

/// <summary>
/// 描画コンポーネントの参照格納
/// </summary>
/// <param name="a_render"></param>
void RenderSystem::RegisterWorldObject(std::shared_ptr<IDrawable> a_render)
{
	if (!a_render)
	{
		return;
	}

	//	レイヤー番号よりコレクションが小さいなら拡張
	int layer = a_render->GetLayer();
	if (layer >= m_worldRenders.size())
	{
		m_worldRenders.resize(layer + 1);
	}

	//	追加
	m_worldRenders[layer].push_back(a_render);
}

/// <summary>
/// UI用の描画コンポーネントの参照格納
/// </summary>
/// <param name="a_render"></param>
void RenderSystem::RegisterScreenObject(std::shared_ptr<IDrawable> a_render)
{
	if (a_render == nullptr)
	{
		return;
	}

	auto layer = a_render->GetLayer();
	if (layer >= m_SceneRenders.size())
	{
		m_SceneRenders.resize(layer + 1);
	}
	m_SceneRenders[layer].push_back(a_render);

}


/// <summary>
/// 描画処理
/// </summary>
void RenderSystem::WorldObjectRender(sf::RenderWindow& a_window, float a_alpha)
{
	for (auto& layer : m_worldRenders)
	{
		for (auto& render : layer)
		{
			//	中身なければ次へ
			if (render.expired() == true)
			{
				continue;
			}

			//	ポインタ取得
			auto ptr = render.lock();

			//	GameObjectがないとき or 無効の時は
			//	処理を行わずに次へ
			if (ptr->GetGameObject() == nullptr ||
				ptr->GetGameObject()->GetEnable() == false)
			{
				continue;
			}

			//	表示可能状態なら描画する
			if (ptr->IsVisible() == true)
			{
				ptr->Draw(a_window,a_alpha);
			}
		} // for 2
	}	//	for 1
}

/// <summary>
/// スクリーンオブジェクトの描画処理
/// </summary>
void RenderSystem::SceneObjectRender(sf::RenderWindow& a_window, float a_alpha)
{
	for (auto& layer : m_SceneRenders)
	{
		for (auto& render : layer)
		{
			//	中身なければ次へ
			if (render.expired() == true)
			{
				continue;
			}

			//	ポインタ取得
			auto ptr = render.lock();

			//	GameObjectがないとき or 無効の時は
			//	処理を行わずに次へ
			if (ptr->GetGameObject() == nullptr ||
				ptr->GetGameObject()->GetEnable() == false)
			{
				continue;
			}

			//	表示可能状態なら描画する
			if (ptr->IsVisible() == true)
			{
				ptr->Draw(a_window, a_alpha);
			}
		} // for 2
	}	//	for 1

}

/// <summary>
/// フレーム末に削除処理
/// </summary>
void RenderSystem::ProcessExpired()
{


	//	レイヤー番号ごとに参照切れがないかを見る
	for (auto& layer : m_worldRenders)
	{
		std::erase_if(layer,
			[](const std::weak_ptr<IDrawable>& render)
			{
				return render.expired();
		});
	}
}

/// <summary>
///	状態のリセット
/// </summary>
void RenderSystem::Clear()
{
	m_worldRenders.clear();
}