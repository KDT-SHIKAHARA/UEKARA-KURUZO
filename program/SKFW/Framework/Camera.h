#pragma once
#include<SKFW/Framework/Singleton.hpp>
#include<SFML/Graphics.hpp>

class Camera : public Singleton<Camera>
{
	friend class Singleton<Camera>;
	Camera() = default;
	virtual~Camera() = default;
public:
	/// <summary>
	/// カメラの初期化
	/// ウィンドウの生成してから呼び出し! これ絶対！
	/// </summary>
	/// <param name="a_windowSize"></param>
	void Initialize(const sf::Vector2u& a_windowSize);

	/// <summary>
	/// カメラの中心セット
	/// </summary>
	/// <param name="a_centerPosition">カメラの座標</param>
	void SetCenter(const sf::Vector2f& a_centerPosition);

	/// <summary>
	/// カメラの相対移動
	/// </summary>
	/// <param name="a_offset">移動量</param>
	void Move(const sf::Vector2f& a_offset);

	/// <summary>
	/// ズーム設定 1.0で等倍
	/// </summary>
	/// <param name="a_zoom"></param>
	void SetZoom(float a_zoom);

	/// <summary>
	/// ワールド用カメラをウィンドウに適用
	/// </summary>
	/// <param name="a_window"></param>
	void ApplyWorldView(sf::RenderWindow& a_window);

	/// <summary>
	/// スクリーンのカメラをウィンドウに適応
	/// </summary>
	/// <param name="a_window"></param>
	void ApplySceneView(sf::RenderWindow& a_window);

	/// <summary>
	/// カメラの取得
	/// </summary>
	/// <returns></returns>
	const sf::View& GetView()const;

private:
	/// <summary>
	/// ワールドのカメラビュー
	/// </summary>
	sf::View m_worldView;

	/// <summary>
	/// UI用の画面のカメラビュー
	/// </summary>
	sf::View m_sceneView;

};