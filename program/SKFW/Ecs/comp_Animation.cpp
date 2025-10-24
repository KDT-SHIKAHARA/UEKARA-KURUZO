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
	//	現在のアニメーションを更新
	if(m_currentAnimationName.empty() == false)
	{
		auto it = m_animations.find(m_currentAnimationName);
		if (it != m_animations.end())
		{
			//	状態更新
			it->second->Update(fixedDt);


			if (it->second->GetType() == IAnimation2D::eType::Frame)
			{
				//	テクスチャが切り替えられたなら張り替える
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
	
	//	補間位置の計算
	m_sprite->setPosition(transform->GetInterpolatedPosition(a_alpha));
	m_sprite->setRotation(transform->GetInterpolatedAngle(a_alpha));
	m_sprite->setScale(transform->GetScale());
	a_window.draw(*m_sprite.get());
}

/// <summary>
/// 複数画像のアニメーションの追加
/// </summary>
/// <param name="a_animKey">アニメーションの名前</param>
/// <param name="a_filePath">ファルダパス</param>
/// <param name="a_firstFrame">最初のフレーム</param>
/// <param name="a_lastFrame">最後のフレーム</param>
/// <param name="a_move">ループか一回限りか</param>
void AnimationComponent::AddFrameAnimation(const std::string& a_animKey, const std::string& a_filePath, int a_firstFrame, int a_lastFrame, IAnimation2D::eMode a_move, IAnimation2D::eType a_type, float a_durationPerFrame)
{
	//	生成してから確認してコレクションに追加
	auto animation = std::make_unique<FrameAnimation2D>(a_filePath, a_firstFrame, a_lastFrame, a_move, a_durationPerFrame);

	//	判定
	if (animation == nullptr)
	{
		return;
	}

	//	空の時だけ現在のアニメーション名をセット
	//	デフォルト設定しないとね。
	if(m_currentAnimationName.empty())
	{
		m_currentAnimationName = a_animKey;
	}

	//	コレクションに追加
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
/// アニメーションの終了判定
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
/// 今のアニメーション名のセット
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
