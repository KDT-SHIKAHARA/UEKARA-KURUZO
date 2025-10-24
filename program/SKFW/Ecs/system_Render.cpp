#include<SKFW/Ecs/system_Render.hpp>

#include<SKFW/Ecs/GameObject.hpp>

/// <summary>
/// �`��R���|�[�l���g�̎Q�Ɗi�[
/// </summary>
/// <param name="a_render"></param>
void RenderSystem::RegisterWorldObject(std::shared_ptr<IDrawable> a_render)
{
	if (!a_render)
	{
		return;
	}

	//	���C���[�ԍ����R���N�V�������������Ȃ�g��
	int layer = a_render->GetLayer();
	if (layer >= m_worldRenders.size())
	{
		m_worldRenders.resize(layer + 1);
	}

	//	�ǉ�
	m_worldRenders[layer].push_back(a_render);
}

/// <summary>
/// UI�p�̕`��R���|�[�l���g�̎Q�Ɗi�[
/// </summary>
/// <param name="a_render"></param>
void RenderSystem::RegisterScreenObject(std::shared_ptr<IDrawable> a_render)
{
	if (a_render == nullptr)
	{
		return;
	}

	auto layer = a_render->GetLayer();
	if (layer >= m_SceneRenders.size())
	{
		m_SceneRenders.resize(layer + 1);
	}
	m_SceneRenders[layer].push_back(a_render);

}


/// <summary>
/// �`�揈��
/// </summary>
void RenderSystem::WorldObjectRender(sf::RenderWindow& a_window, float a_alpha)
{
	for (auto& layer : m_worldRenders)
	{
		for (auto& render : layer)
		{
			//	���g�Ȃ���Ύ���
			if (render.expired() == true)
			{
				continue;
			}

			//	�|�C���^�擾
			auto ptr = render.lock();

			//	GameObject���Ȃ��Ƃ� or �����̎���
			//	�������s�킸�Ɏ���
			if (ptr->GetGameObject() == nullptr ||
				ptr->GetGameObject()->GetEnable() == false)
			{
				continue;
			}

			//	�\���\��ԂȂ�`�悷��
			if (ptr->IsVisible() == true)
			{
				ptr->Draw(a_window,a_alpha);
			}
		} // for 2
	}	//	for 1
}

/// <summary>
/// �X�N���[���I�u�W�F�N�g�̕`�揈��
/// </summary>
void RenderSystem::SceneObjectRender(sf::RenderWindow& a_window, float a_alpha)
{
	for (auto& layer : m_SceneRenders)
	{
		for (auto& render : layer)
		{
			//	���g�Ȃ���Ύ���
			if (render.expired() == true)
			{
				continue;
			}

			//	�|�C���^�擾
			auto ptr = render.lock();

			//	GameObject���Ȃ��Ƃ� or �����̎���
			//	�������s�킸�Ɏ���
			if (ptr->GetGameObject() == nullptr ||
				ptr->GetGameObject()->GetEnable() == false)
			{
				continue;
			}

			//	�\���\��ԂȂ�`�悷��
			if (ptr->IsVisible() == true)
			{
				ptr->Draw(a_window, a_alpha);
			}
		} // for 2
	}	//	for 1

}

/// <summary>
/// �t���[�����ɍ폜����
/// </summary>
void RenderSystem::ProcessExpired()
{


	//	���C���[�ԍ����ƂɎQ�Ɛ؂ꂪ�Ȃ���������
	for (auto& layer : m_worldRenders)
	{
		std::erase_if(layer,
			[](const std::weak_ptr<IDrawable>& render)
			{
				return render.expired();
		});
	}
}

/// <summary>
///	��Ԃ̃��Z�b�g
/// </summary>
void RenderSystem::Clear()
{
	m_worldRenders.clear();
}