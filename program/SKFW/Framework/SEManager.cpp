#include<SKFW/Framework/SEManager.hpp>
#include<SKFW/Framework/SoundResource.hpp>

#include<SKFW/Debug/Logger.hpp>



SEManager::SEManager()
{
	m_sounds.reserve(30);
}

/// <summary>
/// アクティブなリストから再生状態を確認して
/// 終了していたらアクティブリストから削除する
/// </summary>
void SEManager::Update(float a_dt)
{

	//std::erase_if(m_sounds,
	//	[](const sf::Sound& sound)
	//	{ 
	//		return sound.getStatus() == sf::Sound::Status::Stopped; 
	//	}
	//);

	for (auto it = m_sounds.begin(); it != m_sounds.end(); )
	{
		auto& entry = *it;
		if (entry->sound->getStatus() == sf::Sound::Status::Stopped)
		{
			entry->stopTimer += a_dt;
			if (entry->stopTimer > 0.2f)
			{
				it = m_sounds.erase(it);
				continue;
			}
		}
		else
		{
			entry->stopTimer = 0.0f;
		}
		++it;
	}
}

/// <summary>
/// リソースから取得して、失敗した場合はログ出力＋処理を行わない
/// </summary>
/// <param name="a_filePath"></param>
void SEManager::Play(const std::string& a_filePath)
{
	//	リソースの取得
	auto buffer = SoundResource::Instance().GetBuffer(a_filePath);
	if (buffer == nullptr)
	{
		Logger::Warn("SEの再生失敗: %s", a_filePath.c_str());
		return;
	}

	// ヒープ上で sf::Sound を作る
	auto soundPtr = std::make_unique<SoundEntry>();
	soundPtr->sound = std::make_unique<sf::Sound>(*buffer);
	soundPtr->sound->setVolume(m_volume);
	soundPtr->sound->play();
	m_sounds.push_back(std::move(soundPtr));

	Logger::Info("SEの再生成功: %s", a_filePath.c_str());
}

/// <summary>
/// すべてのSEの停止
/// </summary>
void SEManager::AllStop()
{
	for (auto& entry : m_sounds)
	{
		entry->sound->stop();
	}
}


/// <summary>
/// SEの音量変更
/// </summary>
/// <param name="a_volume"></param>
void SEManager::SetVolume(float a_volume)
{
	auto setVolume = std::clamp(a_volume, 0.0f, 100.0f);
	Logger::Info("音量の変更 %f -> %f", m_volume, setVolume);
	m_volume = setVolume;

	for (auto& entry : m_sounds)
	{
		entry->sound->setVolume(m_volume);
	}
}