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
	/// レジスター登録
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 外力の加算
	/// </summary>
	/// <param name="a_force"></param>
	void AddForce(const sf::Vector2f& a_force) noexcept;

	/// <summary>
	/// 移動量をセットする
	/// </summary>
	/// <param name="a_velocity"></param>
	void SetVelocity(const sf::Vector2f& a_velocity)noexcept;

	/// <summary>
	/// 移動量を取得する
	/// </summary>
	/// <returns></returns>
	sf::Vector2f GetVelocity()const;

	/// <summary>
	/// 入力の移動量を加算
	/// </summary>
	/// <param name="a_inputMove"></param>
	void MoveInput(const sf::Vector2f& a_inputMove)noexcept;

	/// <summary>
	/// 再現性のために固定更新
	/// </summary>
	/// <param name="dt"></param>
	void FixedUpdate(float fixedDt)override;

	/// <summary>
	/// 質量を変更する
	/// </summary>
	/// <param name="a_mass"></param>
	void SetMass(float a_mass)noexcept;

	/// <summary>
	/// かかる重力指数を変更する
	/// </summary>
	/// <param name="a_gravity"></param>
	void SetGravity(float a_gravity)noexcept;

	/// <summary>
	/// 移動入力で加える力を変更する
	/// </summary>
	/// <param name="a_moveForce"></param>
	/// <returns></returns>
	void SetMoveForce(float a_moveForce)noexcept;

private:
	/// <summary>
	/// 移動量
	/// </summary>
	sf::Vector2f m_velocity;

	/// <summary>
	/// 加速度
	/// </summary>
	sf::Vector2f m_acceleration;

	/// <summary>
	/// 外力
	/// </summary>
	sf::Vector2f m_force;

	/// <summary>
	/// 質量
	/// </summary>
	float m_mass = 1.0f;

	/// <summary>
	/// 重力加速度
	/// </summary>
	float m_gravity = 980.0f;

	/// <summary>
	/// 空気抵抗
	/// </summary>
	float m_drag_groundX = 0.98f;
	float m_drag_airX = 0.85f; 
	float m_dragY = 0.99f;

	/// <summary>
	/// 移動入力時に加える力
	/// </summary>
	float m_moveForce = 400.0f;
public:
	/// <summary>
	/// 設置判定
	/// </summary>
	Flag m_isGrounded = false;

	/// <summary>
	/// 重力を有効にするかどうか
	/// </summary>
	Flag m_useGravity;
};