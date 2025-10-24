#include<SKFW/Framework/Camera.h>


/// <summary>
/// カメラの初期化
/// </summary>
/// <param name="a_windowSize"></param>
void Camera::Initialize(const sf::Vector2u& a_windowSize)
{
	sf::FloatRect viewRect(
		{0.0f
		,0.0f},
		{static_cast<float>(a_windowSize.x)
		,static_cast<float>(a_windowSize.y)});

	m_sceneView = sf::View(viewRect);
	m_worldView = m_sceneView;
}

/// <summary>
/// カメラの中心セット
/// </summary>
/// <param name="a_centerPosition">カメラの座標</param>
void Camera::SetCenter(const sf::Vector2f& a_centerPosition)
{
	m_worldView.setCenter(a_centerPosition);
}

/// <summary>
/// カメラの相対移動
/// </summary>
/// <param name="a_offset">移動量</param>
void Camera::Move(const sf::Vector2f& a_offset)
{
	m_worldView.move(a_offset);
}

/// <summary>
/// ズーム設定 1.0で等倍
/// </summary>
/// <param name="a_zoom"></param>
void Camera::SetZoom(float a_zoom)
{
	m_worldView.setSize(m_sceneView.getSize() * a_zoom);
}

/// <summary>
/// ワールド用カメラをウィンドウに適用
/// </summary>
/// <param name="a_window"></param>
void Camera::ApplyWorldView(sf::RenderWindow& a_window)
{
	a_window.setView(m_worldView);
}

/// <summary>
/// スクリーンのカメラをウィンドウに適応
/// </summary>
/// <param name="a_window"></param>
void Camera::ApplySceneView(sf::RenderWindow& a_window)
{
	a_window.setView(m_sceneView);
}

/// <summary>
/// カメラの取得
/// </summary>
/// <returns></returns>
const sf::View& Camera::GetView() const
{
	return m_worldView;
}
