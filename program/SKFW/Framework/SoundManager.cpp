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
/// SE�̃A�N�Z�T
/// </summary>
/// <returns></returns>
SEManager& SoundManager::SE()
{
    return m_se;
}

/// <summary>
/// BGM�̃A�N�Z�T
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

    //  �e�Ǘ��̉��ʂ�ύX����
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
/// se�̉��ʕύX
/// </summary>
/// <param name="a_seVolume">�Z�b�g������SE�̉���</param>
void SoundManager::ChangeSEVolume(float a_seVolume)
{
    auto seVolume = std::clamp(a_seVolume, 0.0f, 100.0f);
    m_seVolume = seVolume;
    m_se.SetVolume(this->getFinalVolume(m_seVolume));
}

/// <summary>
/// voice�̉��ʕύX
/// </summary>
/// <param name="a_voiceVolume"></param>
void SoundManager::ChangeVoiceVolume(float a_voiceVolume)
{
    auto volume = std::clamp(a_voiceVolume, 0.0f, 100.0f);
    m_voiceVolume = volume;
    m_voice.SetVolume(this->getFinalVolume(m_voiceVolume));
}
