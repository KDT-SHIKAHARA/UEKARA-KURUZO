#pragma once
#include<SKFW/Ecs/comp_collider.hpp>
#include<SKFW/Ecs/base_Drawable.hpp>

#include<SFML/Graphics/RectangleShape.hpp>
#include<SKFW/Ecs/comp_Transform.hpp>

class DrawRectCollider : public IDrawable
{
public:
	DrawRectCollider(TransformComponent* a_position,
		RectColliderComponent* a_collider);

	/// <summary>
	/// “–‚½‚è”»’è‚Ì•\Ž¦
	/// </summary>
	/// <param name="a_window"></param>
	/// <param name="a_alpha"></param>
	void Draw(sf::RenderWindow& a_window, float a_alpha)override;
private:
	sf::RectangleShape m_rect;
	TransformComponent* m_position;
	RectColliderComponent* m_collider;
};