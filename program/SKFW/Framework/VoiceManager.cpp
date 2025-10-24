#include<SKFW/Framework/VoiceManager.hpp>

#include<SKFW/Framework/SoundResource.hpp>
#include<SKFW/Debug/Logger.hpp>

/// <summary>
/// アクティブなリストから再生状態を確認して
/// 終了していたらアクティブリストから削除する
/// </summary>
void VoiceManager::Update()
{
	std::erase_if(m_sounds, [](const auto& pair) {
		return pair.second.getStatus() == sf::Sound::Status::Stopped;
	});
}

/// <summary>
/// リソースから取得して、失敗した場合はログ出力＋処理を行わない
/// </summary>
/// <param name="a_filePath"></param>
void VoiceManager::Play(const std::string& a_filePath)
{
	auto it = m_sounds.find(a_filePath);
	if (it != m_sounds.end())
	{
		//	再生中なら処理をしない
		if (it->second.getStatus() == sf::Sound::Status::Playing)
		{
			return;
		}
		else
		{
			m_sounds.erase(it);
		}
	}

	//	バッファの読み込み
	auto buffer = SoundResource::Instance().GetBuffer(a_filePath);
	if (buffer == nullptr)
	{
		Logger::Warn("BGMの読み込み失敗: %s", a_filePath.c_str());
		return;
	}

	//	初期化をして
	sf::Sound sound(*buffer);

	//	音量をセット
	sound.setVolume(m_volume);

	//	そのあと再生
	sound.play();

	//	コレクションに追加
	m_sounds.emplace(a_filePath, std::move(sound));
}

/// <summary>
/// すべてのSEの停止
/// </summary>
void VoiceManager::AllStop()
{
	for (auto& [_, sound] : m_sounds)
	{
		sound.stop();
	}
}

/// <summary>
/// SEの音量変更
/// </summary>
/// <param name="a_volume"></param>
void VoiceManager::SetVolume(float a_volume)
{
	auto volume = std::clamp(a_volume, 0.0f, 100.0f);
	Logger::Info("音量の変更 %f -> %f", m_volume, volume);
	m_volume = volume;

	for (auto& [_, sound] : m_sounds)
	{
		sound.setVolume(m_volume);
	}
}