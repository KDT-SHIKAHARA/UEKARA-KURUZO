#include<SKFW/Framework/VoiceManager.hpp>

#include<SKFW/Framework/SoundResource.hpp>
#include<SKFW/Debug/Logger.hpp>

/// <summary>
/// �A�N�e�B�u�ȃ��X�g����Đ���Ԃ��m�F����
/// �I�����Ă�����A�N�e�B�u���X�g����폜����
/// </summary>
void VoiceManager::Update()
{
	std::erase_if(m_sounds, [](const auto& pair) {
		return pair.second.getStatus() == sf::Sound::Status::Stopped;
	});
}

/// <summary>
/// ���\�[�X����擾���āA���s�����ꍇ�̓��O�o�́{�������s��Ȃ�
/// </summary>
/// <param name="a_filePath"></param>
void VoiceManager::Play(const std::string& a_filePath)
{
	auto it = m_sounds.find(a_filePath);
	if (it != m_sounds.end())
	{
		//	�Đ����Ȃ珈�������Ȃ�
		if (it->second.getStatus() == sf::Sound::Status::Playing)
		{
			return;
		}
		else
		{
			m_sounds.erase(it);
		}
	}

	//	�o�b�t�@�̓ǂݍ���
	auto buffer = SoundResource::Instance().GetBuffer(a_filePath);
	if (buffer == nullptr)
	{
		Logger::Warn("BGM�̓ǂݍ��ݎ��s: %s", a_filePath.c_str());
		return;
	}

	//	������������
	sf::Sound sound(*buffer);

	//	���ʂ��Z�b�g
	sound.setVolume(m_volume);

	//	���̂��ƍĐ�
	sound.play();

	//	�R���N�V�����ɒǉ�
	m_sounds.emplace(a_filePath, std::move(sound));
}

/// <summary>
/// ���ׂĂ�SE�̒�~
/// </summary>
void VoiceManager::AllStop()
{
	for (auto& [_, sound] : m_sounds)
	{
		sound.stop();
	}
}

/// <summary>
/// SE�̉��ʕύX
/// </summary>
/// <param name="a_volume"></param>
void VoiceManager::SetVolume(float a_volume)
{
	auto volume = std::clamp(a_volume, 0.0f, 100.0f);
	Logger::Info("���ʂ̕ύX %f -> %f", m_volume, volume);
	m_volume = volume;

	for (auto& [_, sound] : m_sounds)
	{
		sound.setVolume(m_volume);
	}
}