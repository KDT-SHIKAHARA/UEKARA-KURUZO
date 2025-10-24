#pragma once
#include<memory>

#include<SKFW/Debug/DebugTab.hpp>
#include<SKFW/Framework/Singleton.hpp>
#include<SFML/Graphics.hpp>

class DebugUI : public Singleton<DebugUI>
{
	friend class Singleton<DebugUI>;
	DebugUI();
	virtual~DebugUI() = default;
public:
	/// <summary>
	///	ImGuiのウィンドウ生成
	/// </summary>
	/// <param name="window"></param>
	static void Initialize(sf::RenderWindow& window);

	/// <summary>
	/// ImGuiのインスタンス破棄
	/// </summary>
	static void Shutdown();

	/// <summary>
	/// ウィンドウのイベントを取得
	/// </summary>
	/// <param name="window"></param>
	/// <param name="event"></param>
	static void ProcessEvent(sf::Window& window, sf::Event& event);

	/// <summary>
	/// 状態更新
	/// </summary>
	/// <param name="window"></param>
	/// <param name="dt"></param>
	static void Update(sf::RenderWindow& window, sf::Time dt);

	/// <summary>
	/// 描画更新
	/// </summary>
	/// <param name="window"></param>
	static void Render(sf::RenderWindow& window);

	/// <summary>
	/// タブの追加
	/// </summary>
	/// <param name="tab"></param>
	void RegisterTab(std::unique_ptr<DebugTabBase> tab);

	/// <summary>
	///	追加されているタブの表示
	/// </summary>
	void Draw();



private:
	std::vector<std::unique_ptr<DebugTabBase>> m_tabs;
	bool m_isShowDebugUi = true;

};

