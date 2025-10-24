#include<SKFW/Framework/BGMManager.hpp>
#include<SKFW/Debug/Logger.hpp>

/// 
/// /// <summary>
/// �t�F�[�h�Ȃ��؂�ւ�����
/// </summary>
/// <param name="a_filename">�؂�ւ�����BGM�̃t�@�C����</param>
/// <param name="a_loop">���[�v�Đ����邩�ǂ���</param>
void BGMManager::PlayBGMImmediate(const std::string& a_filename, bool a_loop)
{
	//	�t�F�[�h�Ȃ�
	m_isFading = false;

	//	���݂�BGM���~
	if (m_current != nullptr)
	{
		m_current->stop();

		//	�|�C���^�̃��Z�b�g
		std::swap(m_current, m_next);
		m_next = nullptr;
	}

	//	�g�p����Music��I��
	m_current = (m_current == &m_bgmA) ? &m_bgmB : &m_bgmA;

	//	�O��ƈႤ�t�@�C���Ȃ�ǂݍ��ݏ���������
	if (m_currentFile != a_filename)
	{
		bool result = m_current->openFromFile(a_filename);
		if (result == true)
		{
			Logger::Info("BGM�̓ǂݍ��݂ɐ���", a_filename.c_str());
		}
		else
		{
			Logger::Warn("BGM�̓ǂݍ��݂Ɏ��s", a_filename.c_str());
			return;
		}
	}

	//	������������
	m_current->setLooping(a_loop);
	m_current->setVolume(m_volume);
	//	�Đ�
	m_current->play();

}

/// <summary>
/// �t�F�[�h�@�\�t��BGM�Đ�
/// </summary>
/// <param name="a_filename">�؂�ւ�����BGM�̃t�@�C����</param>
/// <param name="a_fadeTime">�t�F�[�h�ɂ����鎞�ԁi�b�j</param>
/// <param name="a_loop">true:���[�v�Đ� false:�P���Đ�</param>
void BGMManager::PlayBGMFade(const std::string& a_filename, float a_fadeTime, bool a_loop)
{
	//	����Đ�����
	if (m_current == nullptr)
	{
		m_current = &m_bgmA;
		auto result = m_current->openFromFile(a_filename);

		//	���O�̓f���o��
		if (result == true)
		{
			Logger::Info("�ǂݍ��ݐ���: %s", a_filename.c_str());
		}
		else
		{
			Logger::Warn("�ǂݍ��ݎ��s: %s", a_filename.c_str());
			m_current = nullptr;
			return;
		}

		m_current->setLooping(a_loop);
		m_current->setVolume(m_volume);
		m_current->play();
	}

	//	����BGM�̃|�C���^�擾
	m_next = (m_current == &m_bgmA) ? &m_bgmB : &m_bgmA;

	//	�O��ƈႤ�t�@�C���Ȃ�ǂݍ��݂�����
	if (m_nextFile != a_filename)
	{
		auto result = m_next->openFromFile(a_filename);
		//	���O�̓f���o��
		if (result == true)
		{
			Logger::Info("�ǂݍ��ݐ���: %s", a_filename.c_str());
		}
		else
		{
			Logger::Warn("�ǂݍ��ݎ��s: %s", a_filename.c_str());
			m_next = nullptr;
			return;
		}

		m_nextFile = a_filename;
	}

	m_next->setLooping(a_loop);
	m_next->setVolume(m_volume);
	m_next->play();
	m_fadeTime = a_fadeTime;
	m_isFading = true;
}

/// <summary>
/// BGM�̒�~�i�t�F�[�h�@�\����j
/// </summary>
/// <param name="a_fadeTime"></param>
void BGMManager::StopBGM(float a_fadeTime)
{
	if (m_current == nullptr)
	{
		return;
	}

	if (a_fadeTime <= 0.0f)
	{
		m_current->stop();
		m_current = nullptr;
		m_next = nullptr;
		m_currentFile.clear();
		m_nextFile.clear();
		m_isFading = false;
	}
	else
	{
		m_next = nullptr;
		m_fadeTime = a_fadeTime;
		m_isFading = true;
	}
}

/// <summary>
/// ��ԍX�V
/// </summary>
/// <param name="dt"></param>
void BGMManager::Update(float dt)
{
	//	�t�F�[�h�Ȃ���Ԃ�BGM���Ȃ��Ƃ��͏������s��Ȃ�
	if (m_isFading == false || m_current == nullptr)
	{
		return;
	}
	
	//	���̉��ʂ̎擾
	float volCurrent = m_current->getVolume();
	float volNext = m_next != nullptr ? m_next->getVolume() : 0.0f;

	float volChange = (m_volume / m_fadeTime) * dt;
	volCurrent = std::max(0.0f, volCurrent - volChange);
	if (m_next != nullptr)
	{
		volNext = std::min(m_volume, volNext + volChange);
	}

	m_current->setVolume(volCurrent);
	if (m_next != nullptr)
	{
		m_next->setVolume(volNext);
	}

	//	�t�F�[�h��������
	if (volCurrent <= 0.0f)
	{
		m_current->stop();
		if (m_next != nullptr)
		{
			m_current = m_next;
			m_currentFile = m_nextFile;
			m_next = nullptr;
			m_nextFile.clear();
		}
		m_isFading = false;
	}
}

/// <summary>
/// ���ʂ̃Z�b�g
/// </summary>
/// <param name="a_volume"></param>
void BGMManager::SetVolume(float a_volume)
{
	auto volume = std::clamp(a_volume, 0.0f, 100.0f);
	if (m_current != nullptr)
	{
		m_current->setVolume(volume);
	}
	if (m_next != nullptr)
	{
		m_next->setVolume(std::min(m_next->getVolume(),m_volume));
	}
}