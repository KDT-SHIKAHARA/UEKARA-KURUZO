#include<SKFW/Ecs/comp_Image.hpp>
#include<SKFW/Ecs/GameObject.hpp>
#include<SKFW/Ecs/comp_Transform.hpp>

ImageComponent::ImageComponent(sf::Texture* a_texture, int a_layer, IDrawable::CoordinateSpace a_coordinateSpace)
	:IDrawable(a_layer, a_coordinateSpace), m_texture(a_texture), m_sprite(*m_texture)
{

}

/// <summary>
/// ‰æ‘œ‚Ì•`‰æ
/// </summary>
/// <param name="a_alpha"></param>
void ImageComponent::Draw(sf::RenderWindow& a_window, float a_alpha)
{
	//	À•WŒn‚ðŽæ“¾‚µ‚Ä”½‰f
	auto transform = GetGameObject()->GetComponent<TransformComponent>();
	if (transform != nullptr)
	{
		auto trans = transform->GetInterpolatedPosition(a_alpha);
		m_sprite.setPosition(trans);
		m_sprite.setRotation(transform->GetInterpolatedAngle(a_alpha));
		m_sprite.setScale(transform->GetScale());
	}

	//	•\Ž¦
	a_window.draw(m_sprite);
}