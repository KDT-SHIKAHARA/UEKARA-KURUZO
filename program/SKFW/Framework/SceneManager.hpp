#pragma once
#include<SKFW/Framework/Scene.hpp>
#include<SKFW/Framework/Singleton.hpp>
#include<SKFW/Debug/Logger.hpp>
#include<SKFW/Framework/base_IOverlay.hpp>

#include<memory>
#include<functional>

class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	virtual ~SceneManager() = default;
public:

	/// <summary>
	/// 初期化、もしシーンのポインタの中身がなかったらデフォルトを生成する
	/// </summary>
	void Initialize();

	/// <summary>
	/// スクリーンの生成（トランジションなし）
	/// </summary>
	template<typename T>
	void CreateScene();

	/// <summary>
	/// スクリーンの切り替え（トランジションなし）
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	void ChangeSceneRequest();

	/// <summary>
	/// トランジション付き画面切り替え
	/// </summary>
	/// <typeparam name="Scene"></typeparam>
	/// <typeparam name="Transition"></typeparam>
	template<typename NEXT,typename Transition>
	void ChangeSceneWithTransition(float a_fadeTime);

	/// <summary>
	/// 可変更新
	/// </summary>
	/// <param name="dt"></param>
	void Update(float a_dt);

	/// <summary>
	/// 固定更新
	/// </summary>
	/// <param name="a_fixedDt"></param>
	void FixedUpdate(float a_fixedDt);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="a_alpha"></param>
	void Render(float a_alpha,sf::RenderWindow& a_window);

	/// <summary>
	/// スクリーンの切り替え判定と切り替え
	/// </summary>
	void ChangeScene();

private:
	/// <summary>
	/// 今のスクリーンのポインタ
	/// </summary>
	std::unique_ptr<Scene> m_scene;

	/// <summary>
	/// オーバーレイのポインタ
	/// </summary>
	std::unique_ptr<OverlayScene> m_overlay;

	/// <summary>
	/// フレーム末でスクリーンを変更する。
	/// 途中で変更すると参照エラーなるので
	/// </summary>
	std::function<void()> m_pendingChange;
};

/// <summary>
/// スクリーンの生成（トランジションなし）
/// </summary>
template<typename T>
inline void SceneManager::CreateScene()
{
	if (std::is_base_of_v<Scene, T> == false)
	{
		Logger::Error("Sceneを継承していない型が生成されそうになりました");
		return;
	}
	m_scene = std::make_unique<T>();
	Logger::Info("Scene: %s が生成されました", m_scene->GetName().c_str());
}

template<typename T>
inline void SceneManager::ChangeSceneRequest()
{
	if (std::is_base_of_v<Scene, T> == false)
	{
		Logger::Error("Sceneを継承していない型が生成されそうになりました");
		return;
	}
	
	//	今のスクリーンの終了処理を呼び出す
	//	GameObjectの返却とか
	if (m_scene != nullptr)
	{
		m_scene->Finalize();
	}

	m_pendingChange = [this]()
	{
		auto scene = std::make_unique<T>();
		scene->Initialize();
		m_scene = std::move(scene);
	};
}

template<typename NEXT, typename Transition>
inline void SceneManager::ChangeSceneWithTransition(float a_fadeTime)
{
	if (std::is_base_of_v<Scene,NEXT> == false)
	{
		Logger::Error("Sceneを継承していない型が生成されそうになりました");
		return;
	}

	if (std::is_base_of_v<OverlayScene, Transition> == false)
	{
		Logger::Error("OverlaySceneを継承していない型が生成されそうになりました");
		return;
	}

	auto scheduleChange = [this]()
	{
			m_pendingChange = [this]()
			{
					m_scene->Finalize();
					auto scene = std::make_unique<NEXT>();
					scene->Initialize();
					m_scene = std::move(scene);
					Logger::Info("Scene: %s が生成されました", m_scene->GetName().c_str());
			};
	};

	m_overlay = std::make_unique<Transition>(scheduleChange, a_fadeTime);
	m_overlay->Initialize();
	Logger::Info("トランジションの生成");

}
