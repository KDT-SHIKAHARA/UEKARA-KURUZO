#pragma once
#include<SKFW/Ecs/base_Drawable.hpp>
#include<SFML/Graphics/Texture.hpp>
#include<SFML/Graphics/Sprite.hpp>

class ImageComponent : public IDrawable
{
public:

	ImageComponent(sf::Texture* a_texture, int a_layer = 0,IDrawable::CoordinateSpace a_coordinateSpace = IDrawable::CoordinateSpace::Screen);

	/// <summary>
	/// 画像の描画
	/// </summary>
	/// <param name="a_alpha"></param>
	void Draw(sf::RenderWindow& a_window, float a_alpha)override;

	//	位置
	void SetPosition(const sf::Vector2f& a_pos) { m_sprite.setPosition(a_pos); }
	const sf::Vector2f& GetPosition()const noexcept { return m_sprite.getPosition(); };

	//	スケール
	void SetScale(const sf::Vector2f& a_scale) noexcept { m_sprite.setScale(a_scale); }
	const sf::Vector2f& GetScale()noexcept { return m_sprite.getScale(); }

	//	回転
	void SetRotation(sf::Angle a_angle)noexcept { m_sprite.setRotation(a_angle); }
	const sf::Angle& GetRotation()const noexcept { return m_sprite.getRotation(); }

	//	色・透明度
	void SetColor(const sf::Color& a_color)noexcept { m_sprite.setColor(a_color); }
	const sf::Color& GetColor()const { return m_sprite.getColor(); }

	//	テクスチャの一部表示
	void SetTextureRect(const sf::IntRect& a_rect) { m_sprite.setTextureRect(a_rect); }

	//	原点
	void SetOrigin(const sf::Vector2f& a_origin)noexcept { m_sprite.setOrigin(a_origin); }

	/// <summary>
	/// デバック操作用にスプライトを渡す。
	/// </summary>
	/// <returns></returns>
	sf::Sprite& GetSprite() { return m_sprite; }

private:
	/// <summary>
	/// リソース管理からすでに読み込んでおいたリソースを管理する
	/// </summary>
	sf::Texture* m_texture;

	/// <summary>
	/// 表示するスプライト
	/// </summary>
	sf::Sprite m_sprite;

};