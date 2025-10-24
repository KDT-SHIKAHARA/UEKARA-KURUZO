#include<SKFW/Ecs/comp_Animation.hpp>
#include<SKFW/Framework/TextureResource.hpp>
#include<SKFW/Ecs/GameObject.hpp>
#include<SKFW/Ecs/comp_Transform.hpp>

void AnimationComponent::FlipAnimation()
{
}

AnimationComponent::AnimationComponent(uint32_t a_layer, IDrawable::CoordinateSpace a_coordinateSpace)
	:IDrawable(a_layer, a_coordinateSpace)
{
}

void AnimationComponent::Initialize()
{
	IDrawable::Initialize();

}

void AnimationComponent::FixedUpdate(float fixedDt)
{
	//	���݂̃A�j���[�V�������X�V
	if(m_currentAnimationName.empty() == false)
	{
		auto it = m_animations.find(m_currentAnimationName);
		if (it != m_animations.end())
		{
			//	��ԍX�V
			it->second->Update(fixedDt);


			if (it->second->GetType() == IAnimation2D::eType::Frame)
			{
				//	�e�N�X�`�����؂�ւ���ꂽ�Ȃ璣��ւ���
				if (it->second->m_hasChanged == true)
				{
					m_sprite->setTexture(*it->second->GetTexture());
				}
			}
			
		}

	}
}

void AnimationComponent::Draw(sf::RenderWindow& a_window, float a_alpha)
{
	auto transform = GetGameObject()->GetComponent<TransformComponent>();
	if (transform == nullptr)
	{
		return;
	}
	
	//	��Ԉʒu�̌v�Z
	m_sprite->setPosition(transform->GetInterpolatedPosition(a_alpha));
	m_sprite->setRotation(transform->GetInterpolatedAngle(a_alpha));
	m_sprite->setScale(transform->GetScale());
	a_window.draw(*m_sprite.get());
}

/// <summary>
/// �����摜�̃A�j���[�V�����̒ǉ�
/// </summary>
/// <param name="a_animKey">�A�j���[�V�����̖��O</param>
/// <param name="a_filePath">�t�@���_�p�X</param>
/// <param name="a_firstFrame">�ŏ��̃t���[��</param>
/// <param name="a_lastFrame">�Ō�̃t���[��</param>
/// <param name="a_move">���[�v�������肩</param>
void AnimationComponent::AddFrameAnimation(const std::string& a_animKey, const std::string& a_filePath, int a_firstFrame, int a_lastFrame, IAnimation2D::eMode a_move, IAnimation2D::eType a_type, float a_durationPerFrame)
{
	//	�������Ă���m�F���ăR���N�V�����ɒǉ�
	auto animation = std::make_unique<FrameAnimation2D>(a_filePath, a_firstFrame, a_lastFrame, a_move, a_durationPerFrame);

	//	����
	if (animation == nullptr)
	{
		return;
	}

	//	��̎��������݂̃A�j���[�V���������Z�b�g
	//	�f�t�H���g�ݒ肵�Ȃ��ƂˁB
	if(m_currentAnimationName.empty())
	{
		m_currentAnimationName = a_animKey;
	}

	//	�R���N�V�����ɒǉ�
	m_animations.emplace(a_animKey, std::move(animation));

	if (m_sprite == nullptr)
	{
		m_animations[a_animKey]->SetType(a_type);
		m_sprite = std::make_unique<sf::Sprite>(*m_animations[a_animKey]->GetTexture());
		
		if (m_gameObject != nullptr)
		{
			auto transform = m_gameObject->GetComponent<TransformComponent>();
			if (transform != nullptr)
			{
				if (transform->GetPivot() == TransformComponent::ePivot::Center)
				{
					m_sprite->setOrigin({ m_sprite->getLocalBounds().size.x / 2, m_sprite->getLocalBounds().size.y / 2 });
				}
			}
		}

	}
}

/// <summary>
/// �A�j���[�V�����̏I������
/// </summary>
/// <returns></returns>
bool AnimationComponent::IsAnimationFinished() const noexcept
{
	if(m_currentAnimationName.empty())
	{
		return false;
	}

	return m_animations.at(m_currentAnimationName)->m_isFinished;
}

/// <summary>
/// ���̃A�j���[�V�������̃Z�b�g
/// </summary>
/// <param name="a_name"></param>
void AnimationComponent::SetAnimationKey(const std::string& a_name)
{
	auto it = m_animations.find(a_name);
	if(it != m_animations.end())
	{
		it->second->Reset();
		m_currentAnimationName = a_name;
	}
}
