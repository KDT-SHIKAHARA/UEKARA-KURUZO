#pragma once

#include<memory>
#include<vector>

#include<SKFW/Ecs/base_Drawable.hpp>
#include<SKFW/Framework/Singleton.hpp>
#include<SFML/Graphics/RenderWindow.hpp>

/// <summary>
/// �`��R���|�[�l���g�̕`�揈����S��
/// </summary>
class RenderSystem : public Singleton<RenderSystem>
{
	friend class Singleton<RenderSystem>;
	RenderSystem() = default;
	virtual ~RenderSystem() = default;
public:

	/// <summary>
	/// ���[���h��̕`��R���|�[�l���g�̎Q�Ɗi�[
	/// </summary>
	/// <param name="a_render"></param>
	void RegisterWorldObject(std::shared_ptr<IDrawable> a_render);

	/// <summary>
	/// UI�p�̕`��R���|�[�l���g�̎Q�Ɗi�[
	/// </summary>
	/// <param name="a_render"></param>
	void RegisterScreenObject(std::shared_ptr<IDrawable> a_render);


	/// <summary>
	/// ���[���h�I�u�W�F�N�g�̕`�揈��
	/// </summary>
	void WorldObjectRender(sf::RenderWindow& a_window,float a_alpha);

	/// <summary>
	/// �X�N���[���I�u�W�F�N�g�̕`�揈��
	/// </summary>
	/// <param name="a_window"></param>
	/// <param name="a_alpha"></param>
	void SceneObjectRender(sf::RenderWindow& a_window, float a_alpha);

	/// <summary>
	/// �t���[�����ɍ폜����
	/// </summary>
	void ProcessExpired();

	/// <summary>
	///	��Ԃ̃��Z�b�g
	/// </summary>
	void Clear();

private:
	/// <summary>
	/// �`�悷��R���|�[�l���g�̎Q�ƃR���N�V����
	/// </summary>
	std::vector<std::vector<std::weak_ptr<IDrawable>>> m_worldRenders;
	std::vector<std::vector<std::weak_ptr<IDrawable>>> m_SceneRenders;

};