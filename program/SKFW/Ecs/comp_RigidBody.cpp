#include<SKFW/Ecs/comp_RigidBody.hpp>
#include<SKFW/Ecs/GameObject.hpp>
#include<SKFW/Ecs/comp_Transform.hpp>

#include<SKFW/Ecs/system_RigidBody.hpp>

#include<algorithm>
#include<cmath>

RigidBodyComponent::RigidBodyComponent(bool a_useGravity)
	:m_useGravity(a_useGravity)
{
}

void RigidBodyComponent::Initialize()
{
	RigidBodySystem::Instance().Register(shared_from_this());
}

/// <summary>
/// 外力の加算
/// </summary>
/// <param name="a_force"></param>
void RigidBodyComponent::AddForce(const sf::Vector2f& a_force) noexcept
{
	m_force += a_force;
}

void RigidBodyComponent::SetVelocity(const sf::Vector2f& a_velocity) noexcept
{
	m_velocity = a_velocity;
}

sf::Vector2f RigidBodyComponent::GetVelocity() const
{
	return m_velocity;
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
	auto transform = GetGameObject()->GetComponent<TransformComponent>();

	if (transform == nullptr)
	{
		return;
	}

	//	着地していないときに
	if (m_isGrounded == false)
	{
		//	重力
		if (m_useGravity == true)
		{
			m_force.y += m_mass * m_gravity;
		}
	}

	//	加速度 = 力 /  質量
	m_acceleration = m_force * (1.0f / m_mass);

	//	速度更新
	m_velocity += m_acceleration * fixedDt;


	//	空気抵抗（空中と地上の滑りやすさ）
	auto dragX = m_isGrounded == true ? m_drag_groundX : m_drag_airX;
	auto fps = 1.0f / fixedDt;

	// Y方向の減衰
	float dragY = m_dragY;
	if (!m_useGravity) {
		// 重力OFFならY軸も横と同じ抵抗にする
		dragY = dragX;
	}
	m_velocity.x *= dragX;
	m_velocity.y *= dragY;

	//	位置の更新
	transform->Translate(m_velocity * fixedDt);


	//	力のリセット
	m_force = {0,0};
}

void RigidBodyComponent::SetMass(float a_mass) noexcept
{
	m_mass = (a_mass <= 0.0f) ? 1.0f : a_mass;
}

void RigidBodyComponent::SetGravity(float a_gravity) noexcept
{
	m_gravity = a_gravity;
}

void RigidBodyComponent::SetMoveForce(float a_moveForce)noexcept
{
	m_moveForce = a_moveForce;
}
