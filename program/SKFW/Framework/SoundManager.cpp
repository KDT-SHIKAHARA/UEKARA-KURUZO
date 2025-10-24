#include "SoundManager.hpp"

float SoundManager::getFinalVolume(float a_volume)
{
    float volume = a_volume * (m_masterVolume / 100.0f);
    volume = std::clamp(volume, 0.0f, 1.0f);
    return 0.0f;
}


void SoundManager::Update(float dt)
{
    m_se.Update(dt);
    m_bgm.Update(dt);
    m_voice.Update();
}

/// <summary>
/// SEのアクセサ
/// </summary>
/// <returns></returns>
SEManager& SoundManager::SE()
{
    return m_se;
}

/// <summary>
/// BGMのアクセサ
/// </summary>
/// <returns></returns>
BGMManager& SoundManager::BGM()
{
    return m_bgm;
}

void SoundManager::ChangeMasterVolume(float a_masterVolume)
{
    auto masterVolume = std::clamp(a_masterVolume, 0.0f, 100.0f);
    m_masterVolume = masterVolume;

    //  各管理の音量を変更する
    m_se.SetVolume(this->getFinalVolume(m_seVolume));
    m_bgm.SetVolume(this->getFinalVolume(m_bgmVolume));
    m_voice.SetVolume(this->getFinalVolume(m_voiceVolume));
}

void SoundManager::ChangeBGMVolume(float a_bgmVolume)
{
    auto volume = std::clamp(m_bgmVolume, 0.0f, 100.0f);
    m_bgmVolume = volume;
    m_bgm.SetVolume(this->getFinalVolume(m_bgmVolume));
}

/// <summary>
/// seの音量変更
/// </summary>
/// <param name="a_seVolume">セットしたいSEの音量</param>
void SoundManager::ChangeSEVolume(float a_seVolume)
{
    auto seVolume = std::clamp(a_seVolume, 0.0f, 100.0f);
    m_seVolume = seVolume;
    m_se.SetVolume(this->getFinalVolume(m_seVolume));
}

/// <summary>
/// voiceの音量変更
/// </summary>
/// <param name="a_voiceVolume"></param>
void SoundManager::ChangeVoiceVolume(float a_voiceVolume)
{
    auto volume = std::clamp(a_voiceVolume, 0.0f, 100.0f);
    m_voiceVolume = volume;
    m_voice.SetVolume(this->getFinalVolume(m_voiceVolume));
}
