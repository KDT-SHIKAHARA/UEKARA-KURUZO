#include<SKFW/Framework/GameManager.hpp>
#include<SKFW/Framework/Time.hpp>
#include<SKFW/Framework/InputManager.hpp>
#include<SKFW/Debug/DebugUi.hpp>
#include<SKFW/Framework/data_window.h>
#include<SKFW/Framework/Camera.h>

#include<SKFW/Ecs/GameObjectManager.hpp>
#include<SKFW/Framework/TextureResource.hpp>
#include<SKFW/Framework/SoundResource.hpp>
#include<SKFW/Framework/SoundManager.hpp>

#include<SKFW/Ecs/system_Transform.hpp>
#include<SKFW/Ecs/system_Render.hpp>
#include<SKFW/Ecs/system_RigidBody.hpp>
#include<SKFW/Ecs/system_Collision.hpp>

#include<SKFW/Framework/SceneManager.hpp>

#include<SKFW/Debug/Logger.hpp>
#include<SKFW/Game/TitleScene.h>

#include<SKFW/Framework/TestScene.h>
#include<SKFW/Framework/TransitionOverlay.h>

GameManager::GameManager()
	:m_running(true), m_fps(240.0f), m_fixedFps(60.0f)
{
}

void GameManager::Update(float a_dt)
{
	//	Behavirourの更新
	SceneManager::Instance().Update(a_dt);

	//	サウンドの状態更新
	SoundManager::Instance().Update(a_dt);

	//if (InputManager::Instance().IsKeyPressed(sf::Keyboard::Key::F))
	//{
	//	SceneManager::Instance().ChangeSceneWithTransition<TestScene, TransitionOverlay>(1.0f);
	//}
}

void GameManager::FixedUpdate(float a_fixedDt)
{
	//	Behavirourの固定更新
	//	移動処理
	//	当たり判定
	SceneManager::Instance().FixedUpdate(a_fixedDt);
}

void GameManager::Render(float a_alpha)
{
	//	ワールド
	Camera::Instance().ApplyWorldView(m_window);
	RenderSystem::Instance().WorldObjectRender(m_window, a_alpha);

	//	スクリーン
	Camera::Instance().ApplySceneView(m_window);
	RenderSystem::Instance().SceneObjectRender(m_window, a_alpha);
}

void GameManager::HandleEvents()
{
	while (auto event = m_window.pollEvent())
	{
		DebugUI::ProcessEvent(m_window, *event);
		InputManager::Instance().ProcessEvent(*event);

		//	終了処理
		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
		}
	}
}

/// <summary>
/// 目標のfps
/// </summary>
/// <param name="a_fps"></param>
void GameManager::SetFps(float a_fps)
{
	if (a_fps < 30)
	{
		a_fps = 30;
	}

	m_fps = a_fps;
}

/// <summary>
/// ウィンドウの作成、各システムの生成、初期化
/// </summary>
void GameManager::Initialize()
{
	//	ログの初期化
	Logger::Initialize();


	//	開始ログを出力
	Logger::Info("ゲーム起動");

	//	データ読み込み
	Logger::Info("データ読み込み開始");
	TextureResourceManager::Instance().LoadAllFromDirectory("assets/textures/");
	SoundResource::Instance().LoadAllFromDirectory("assets/sounds/");

	//SoundResource::Instance().UnloadAllFromDirectory("assets/sounds/");

	//	ウィンドウ初期化
	//	必要だったらウィンドウ用のconfigファイルを追加してデータを取得してから作成してもいい
	{
		Logger::Info("ウィンドウの作成");
		sf::State mode = sf::State::Windowed;
		m_window = sf::RenderWindow(WindowData::m_video, WindowData::m_windowName, sf::Style::Default | sf::Style::Resize, WindowData::m_mode);
		m_window.setFramerateLimit(m_fps);
		DebugUI::Initialize(m_window);
	}

	//	この時点で最初のシーンを生成しておく
	SceneManager::Instance().CreateScene<TitleScene>();

	//	初期化
	SceneManager::Instance().Initialize();

	//	時間管理初期化
	Time::Instance().Initialize(m_fixedFps);

	//	カメラの初期化
	Camera::Instance().Initialize(m_window.getSize());

	//	開始ログを出力
	Logger::Info("ゲーム開始時のフレームレート設定 FPS: %.2f, Fixed FPS: %.2f", m_fps, m_fixedFps);

}

/// <summary>
/// ゲームループ、各システムの状態更新、描画更新を行う。
/// </summary>
void GameManager::Run()
{
	auto& time = Time::Instance();
	Logger::Info("ゲームループの開始");

	//	ループ
	while (m_window.isOpen() && m_running == true)
	{
		//	ウィンドウのイベント関係
		HandleEvents();

		//	先にフレーム開始地点で時間経過しないといけないので
		time.Update();


		//	ImGuiの内部状態更新
		DebugUI::Update(m_window, sf::seconds(time.DeltaTime()));


		//	可変更新
		this->Update(time.DeltaTime());

		while (time.ShouldFixedUpdate())
		{

			//	座標系の全体の状態を保存
			TransformSystem::Instance().SavePrevious();

			//	固定更新
			this->FixedUpdate(time.FixedDelta());
	
			//	固定更新の時間経過
			time.ConsumeFixedDelta();
		}

		Time::Instance().UpdateAlpha();
		
		m_window.clear(sf::Color(64,64,64));

		//	補間を渡して滑らかにして描画する
		SceneManager::Instance().Render(time.Alpha(), m_window);

		//	ImGui描画
		DebugUI::Instance().Draw();
		DebugUI::Render(m_window);

		m_window.display();

		InputManager::Instance().Update();

		//	スクリーンの切り替え
		SceneManager::Instance().ChangeScene();

		//	削除ループ
		GameObjectManager::Instance().CleanupDestroyedObjects();
		TransformSystem::Instance().CleanupDestroyed();
		RenderSystem::Instance().ProcessExpired();
		RigidBodySystem::Instance().CleanupDestroyed();
	}

	Logger::Info("ゲームループの終了");


}

/// <summary>
/// 終了処理、ウィンドウの破棄、フレームワークの削除制御が必要なら行う
/// </summary>
void GameManager::Finalize()
{
	DebugUI::Shutdown();
	Logger::Info("ゲームの正常終了");
}

/// <summary>
/// ゲームループの終了
/// </summary>
void GameManager::Stop()
{
	m_running = false;
}
