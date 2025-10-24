#include<SKFW/Debug/DebugUi.hpp>
#include<SKFW/Framework/InputManager.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#ifdef _DEBUG
#define DEBUGUI_ENABLED 1
#else
#define DEBUGUI_ENABLED 0
#endif // _DEBUG

#include<SKFW/Debug/debug_GameManager.hpp>
#include<SKFW/Debug/debug_TagManager.hpp>
#include<SKFW/Debug/debug_GameObjectManager.hpp>
#include<SKFW/Debug/debug_GamePad.h>

DebugUI::DebugUI()
{
	this->RegisterTab(std::make_unique<DebugGameManager>());
	this->RegisterTab(std::make_unique<DebugTagManager>());
	this->RegisterTab(std::make_unique<DebugGamePad>());
	this->RegisterTab(std::make_unique<DebugGameObjectManager>());
}

/// <summary>
///	ImGuiのウィンドウ生成
/// </summary>
/// <param name="window"></param>
void DebugUI::Initialize(sf::RenderWindow& window)
{
#if DEBUGUI_ENABLED
	ImGui::SFML::Init(window);
#endif // DEBUGUI_ENABLED
}

/// <summary>
/// ImGuiのインスタンス破棄
/// </summary>
void DebugUI::Shutdown()
{
#if DEBUGUI_ENABLED
	ImGui::SFML::Shutdown();
#endif // DEBUGUI_ENABLED
}

/// <summary>
/// ウィンドウのイベントを取得
/// </summary>
/// <param name="window"></param>
/// <param name="event"></param>
void DebugUI::ProcessEvent(sf::Window& window, sf::Event& event)
{
#if DEBUGUI_ENABLED
	ImGui::SFML::ProcessEvent(window, event);
#endif // DEBUGUI_ENABLED
}

/// <summary>
/// 状態更新
/// </summary>
/// <param name="window"></param>
/// <param name="dt"></param>
void DebugUI::Update(sf::RenderWindow& window, sf::Time dt)
{
#if DEBUGUI_ENABLED
	ImGui::SFML::Update(window, dt);
#endif // DEBUGUI_ENABLED
}

/// <summary>
/// 描画更新
/// </summary>
/// <param name="window"></param>
void DebugUI::Render(sf::RenderWindow& window)
{
#if DEBUGUI_ENABLED
	ImGui::SFML::Render(window);
#endif // DEBUGUI_ENABLED
}

/// <summary>
/// タブの追加
/// </summary>
/// <param name="tab"></param>
void DebugUI::RegisterTab(std::unique_ptr<DebugTabBase> tab)
{
#if DEBUGUI_ENABLED
	m_tabs.push_back(std::move(tab));
#endif // DEBUGUI_ENABLED

}

/// <summary>
///	追加されているタブの表示
/// </summary>
void DebugUI::Draw()
{
#if DEBUGUI_ENABLED

	//	F1で表示フラグの切り替え
	if (InputManager::Instance().IsKeyPressed(sf::Keyboard::Key::F1)) {
		m_isShowDebugUi = !m_isShowDebugUi;
	}

	//	表示フラグが立っているときだけ
	if (m_isShowDebugUi) {
		ImGui::Begin("debug");

		if (ImGui::BeginTabBar("debug tab", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_AutoSelectNewTabs)) {
			for (auto& tab : m_tabs) {
				if (ImGui::BeginTabItem(tab->GetName().c_str())) {
					tab->Draw();
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
	}

#endif // DEBUGUI_ENABLED
}
