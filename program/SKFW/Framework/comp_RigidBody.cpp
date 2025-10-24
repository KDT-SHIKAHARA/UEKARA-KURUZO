#include "comp_RigidBody.h"

#include<algorithm>
#include<cmath>

/// <summary>
/// 外力の加算
/// </summary>
/// <param name="a_force"></param>
void RigidBodyComponent::AddForce(const sf::Vector2f& a_force) noexcept
{
	m_force += a_force;
}

/// <summary>
/// 入力の移動量を加算
/// </summary>
/// <param name="a_inputMove"></param>
void RigidBodyComponent::MoveInput(const sf::Vector2f& a_inputMove) noexcept
{
	sf::Vector2f move = a_inputMove * m_moveForce;
	AddForce(move);
}

/// <summary>
/// 再現性のために固定更新
/// </summary>
/// <param name="dt"></param>
void RigidBodyComponent::FixedUpdate(float fixedDt)
{
	//	座標のコンポーネントの取得
	auto transform = GetGameObject();

	//	加速度 = 力 /  質量
	m_acceleration = m_force * (1.0f / m_mass);

	//	速度更新
	m_velociry += m_acceleration * fixedDt;

	//	空気抵抗（空中と地上の滑りやすさ）
	auto dragX = m_isGrounded == true ? m_drag_groundX : m_drag_airX;
	auto fps = 1.0f / fixedDt;
	m_velociry.x *= std::pow(dragX, fixedDt * fps);
	m_velociry.y *= std::pow(m_dragY, fixedDt * fps);

	//	位置の更新

	//	力のリセット
}
