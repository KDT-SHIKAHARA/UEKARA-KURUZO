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
/// �O�͂̉��Z
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
/// ���͂̈ړ��ʂ����Z
/// </summary>
/// <param name="a_inputMove"></param>
void RigidBodyComponent::MoveInput(const sf::Vector2f& a_inputMove) noexcept
{
	sf::Vector2f move = a_inputMove * m_moveForce;
	AddForce(move);
}

/// <summary>
/// �Č����̂��߂ɌŒ�X�V
/// </summary>
/// <param name="dt"></param>
void RigidBodyComponent::FixedUpdate(float fixedDt)
{
	//	���W�̃R���|�[�l���g�̎擾
	auto transform = GetGameObject()->GetComponent<TransformComponent>();

	if (transform == nullptr)
	{
		return;
	}

	//	���n���Ă��Ȃ��Ƃ���
	if (m_isGrounded == false)
	{
		//	�d��
		if (m_useGravity == true)
		{
			m_force.y += m_mass * m_gravity;
		}
	}

	//	�����x = �� /  ����
	m_acceleration = m_force * (1.0f / m_mass);

	//	���x�X�V
	m_velocity += m_acceleration * fixedDt;


	//	��C��R�i�󒆂ƒn��̊���₷���j
	auto dragX = m_isGrounded == true ? m_drag_groundX : m_drag_airX;
	auto fps = 1.0f / fixedDt;

	// Y�����̌���
	float dragY = m_dragY;
	if (!m_useGravity) {
		// �d��OFF�Ȃ�Y�������Ɠ�����R�ɂ���
		dragY = dragX;
	}
	m_velocity.x *= dragX;
	m_velocity.y *= dragY;

	//	�ʒu�̍X�V
	transform->Translate(m_velocity * fixedDt);


	//	�͂̃��Z�b�g
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
