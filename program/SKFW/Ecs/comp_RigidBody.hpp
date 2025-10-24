#pragma once
#include<memory>

#include<SKFW/Ecs/base_component.hpp>
#include<SFML/System/Vector2.hpp>
#include<SKFW/Framework/Flag.h>

class RigidBodyComponent : public IComponent , public std::enable_shared_from_this<RigidBodyComponent>
{
public:
	RigidBodyComponent(bool a_useGravity = false);

	/// <summary>
	/// ���W�X�^�[�o�^
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// �O�͂̉��Z
	/// </summary>
	/// <param name="a_force"></param>
	void AddForce(const sf::Vector2f& a_force) noexcept;

	/// <summary>
	/// �ړ��ʂ��Z�b�g����
	/// </summary>
	/// <param name="a_velocity"></param>
	void SetVelocity(const sf::Vector2f& a_velocity)noexcept;

	/// <summary>
	/// �ړ��ʂ��擾����
	/// </summary>
	/// <returns></returns>
	sf::Vector2f GetVelocity()const;

	/// <summary>
	/// ���͂̈ړ��ʂ����Z
	/// </summary>
	/// <param name="a_inputMove"></param>
	void MoveInput(const sf::Vector2f& a_inputMove)noexcept;

	/// <summary>
	/// �Č����̂��߂ɌŒ�X�V
	/// </summary>
	/// <param name="dt"></param>
	void FixedUpdate(float fixedDt)override;

	/// <summary>
	/// ���ʂ�ύX����
	/// </summary>
	/// <param name="a_mass"></param>
	void SetMass(float a_mass)noexcept;

	/// <summary>
	/// ������d�͎w����ύX����
	/// </summary>
	/// <param name="a_gravity"></param>
	void SetGravity(float a_gravity)noexcept;

	/// <summary>
	/// �ړ����͂ŉ�����͂�ύX����
	/// </summary>
	/// <param name="a_moveForce"></param>
	/// <returns></returns>
	void SetMoveForce(float a_moveForce)noexcept;

private:
	/// <summary>
	/// �ړ���
	/// </summary>
	sf::Vector2f m_velocity;

	/// <summary>
	/// �����x
	/// </summary>
	sf::Vector2f m_acceleration;

	/// <summary>
	/// �O��
	/// </summary>
	sf::Vector2f m_force;

	/// <summary>
	/// ����
	/// </summary>
	float m_mass = 1.0f;

	/// <summary>
	/// �d�͉����x
	/// </summary>
	float m_gravity = 980.0f;

	/// <summary>
	/// ��C��R
	/// </summary>
	float m_drag_groundX = 0.98f;
	float m_drag_airX = 0.85f; 
	float m_dragY = 0.99f;

	/// <summary>
	/// �ړ����͎��ɉ������
	/// </summary>
	float m_moveForce = 400.0f;
public:
	/// <summary>
	/// �ݒu����
	/// </summary>
	Flag m_isGrounded = false;

	/// <summary>
	/// �d�͂�L���ɂ��邩�ǂ���
	/// </summary>
	Flag m_useGravity;
};