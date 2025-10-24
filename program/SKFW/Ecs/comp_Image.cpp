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
    auto transform = GetGameObject()->GetComponent<TransformComponent>();
    if (transform != nullptr)
    {
        // Pivot‚É‰ž‚¶‚ÄŒ´“_‚ðÝ’è
        if (transform->GetPivot() == TransformComponent::ePivot::Center)
        {
            m_sprite.setOrigin({ m_sprite.getLocalBounds().size.x / 2, m_sprite.getLocalBounds().size.y / 2 });
        }

        // •âŠÔÀ•W‚ð”½‰f
        m_sprite.setPosition(transform->GetInterpolatedPosition(a_alpha));
        m_sprite.setRotation(transform->GetInterpolatedAngle(a_alpha));
        m_sprite.setScale(transform->GetScale());
    }

    // •`‰æ
    a_window.draw(m_sprite);
}