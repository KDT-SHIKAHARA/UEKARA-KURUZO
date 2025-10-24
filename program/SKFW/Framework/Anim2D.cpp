#include<SKFW/Framework/TextureResource.hpp>
#include<SKFW/Framework/Anim2D.hpp>
#include<SKFW/Debug/Logger.hpp>

#include<cassert>

FrameAnimation2D::FrameAnimation2D(const std::string& a_filepath, int a_firstFrame, int a_lastFrame, IAnimation2D::eMode a_mode,float a_durationPerFrame)
	:IAnimation2D(a_mode, a_durationPerFrame)
	,m_firstFrame(a_firstFrame)
	, m_lastFrame(a_lastFrame)
	, m_currentFrame(a_firstFrame)
{
	// テクスチャ群の取得
	for (int i = a_firstFrame; i <= a_lastFrame; i++)
	{
		std::string filepath = a_filepath + std::to_string(i) + ".png";
		auto texture = TextureResourceManager::Instance().GetTexture(filepath);
		
		//	失敗処理
		if (texture == nullptr)
		{
			Logger::Info("2Dアニメーション用テクスチャ読込失敗:", filepath.c_str());
		}
		assert(texture != nullptr && "2Dアニメーションでテクスチャの読込に失敗");

		m_textures.push_back(texture);
	}
}

/// <summary>
/// アニメーションの更新処理
/// </summary>
/// <param name="a_fixedDt"></param>
void FrameAnimation2D::Update(double a_fixedDt)
{
	// 再生していない、またはテクスチャがない場合は処理しない
	if(m_isPlaying == Flag::Off || m_textures.empty())
	{ 
		return;
	}

	//	テクスチャ変更フラグの設定
	m_hasChanged.Set(Flag::Off);

	//	経過時間の更新
	m_elapsedTime += a_fixedDt;

	while (m_elapsedTime >= m_durationPerFrame)
	{
		m_elapsedTime -= m_durationPerFrame;
		stepFrame();
		if(m_isPlaying == Flag::Off)
		{
			break;
		}
	}
}

/// <summary>
/// 切り替え時のリセット処理
/// </summary>
void FrameAnimation2D::Reset()
{
	m_elapsedTime = 0;
	m_currentFrame = 0;
	m_isPlaying.Set(Flag::On);
	m_isFinished.Set(Flag::Off);
	m_hasChanged.Set(Flag::Off);
}

/// <summary>
/// テクスチャの取得
/// </summary>
/// <returns></returns>
const sf::Texture* FrameAnimation2D::GetTexture() const
{
	if (m_textures.empty())
	{
		Logger::Warn("テクスチャ取得時にテクスチャが存在しませんでした。");
		return nullptr;
	}

	return m_textures[m_currentFrame];
}

/// <summary>
/// アニメーションのフレームを進める
/// </summary>
void FrameAnimation2D::stepFrame()
{

	switch (m_playMode)
	{
	//	単発処理
	//	こっちは範囲外になるとそのままのなので増やす前に判定をする
	case IAnimation2D::eMode::Once:
		//	次のアニメフレームがサイズを越してるなら終了
		if (m_currentFrame + 1 >= m_textures.size())
		{
			m_isPlaying.Set(Flag::Off);
			m_isFinished.Set(Flag::On);
		}
		//	越してないなら進める
		else
		{
			m_currentFrame++;
			m_hasChanged.Set(Flag::On);
		}

		break;
	//	ループ処理
	//	こっちはClampを実質しているので常に範囲内なので加算を先にします
	case IAnimation2D::eMode::Loop:
		m_currentFrame++;
		m_currentFrame = m_currentFrame % m_textures.size();	
		m_hasChanged.Set(Flag::On);
		break;
	}

}
