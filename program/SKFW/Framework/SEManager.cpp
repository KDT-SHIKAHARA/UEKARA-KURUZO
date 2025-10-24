#include<SKFW/Framework/SEManager.hpp>
#include<SKFW/Framework/SoundResource.hpp>

#include<SKFW/Debug/Logger.hpp>



SEManager::SEManager()
{
	m_sounds.reserve(30);
}

/// <summary>
/// �A�N�e�B�u�ȃ��X�g����Đ���Ԃ��m�F����
/// �I�����Ă�����A�N�e�B�u���X�g����폜����
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
/// ���\�[�X����擾���āA���s�����ꍇ�̓��O�o�́{�������s��Ȃ�
/// </summary>
/// <param name="a_filePath"></param>
void SEManager::Play(const std::string& a_filePath)
{
	//	���\�[�X�̎擾
	auto buffer = SoundResource::Instance().GetBuffer(a_filePath);
	if (buffer == nullptr)
	{
		Logger::Warn("SE�̍Đ����s: %s", a_filePath.c_str());
		return;
	}

	// �q�[�v��� sf::Sound �����
	auto soundPtr = std::make_unique<SoundEntry>();
	soundPtr->sound = std::make_unique<sf::Sound>(*buffer);
	soundPtr->sound->setVolume(m_volume);
	soundPtr->sound->play();
	m_sounds.push_back(std::move(soundPtr));

	Logger::Info("SE�̍Đ�����: %s", a_filePath.c_str());
}

/// <summary>
/// ���ׂĂ�SE�̒�~
/// </summary>
void SEManager::AllStop()
{
	for (auto& entry : m_sounds)
	{
		entry->sound->stop();
	}
}


/// <summary>
/// SE�̉��ʕύX
/// </summary>
/// <param name="a_volume"></param>
void SEManager::SetVolume(float a_volume)
{
	auto setVolume = std::clamp(a_volume, 0.0f, 100.0f);
	Logger::Info("���ʂ̕ύX %f -> %f", m_volume, setVolume);
	m_volume = setVolume;

	for (auto& entry : m_sounds)
	{
		entry->sound->setVolume(m_volume);
	}
}