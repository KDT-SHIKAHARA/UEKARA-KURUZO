#include<SKFW/Framework/BGMManager.hpp>
#include<SKFW/Debug/Logger.hpp>

/// 
/// /// <summary>
/// フェードなし切り替え処理
/// </summary>
/// <param name="a_filename">切り替えたいBGMのファイル名</param>
/// <param name="a_loop">ループ再生するかどうか</param>
void BGMManager::PlayBGMImmediate(const std::string& a_filename, bool a_loop)
{
	//	フェードなし
	m_isFading = false;

	//	現在のBGMを停止
	if (m_current != nullptr)
	{
		m_current->stop();

		//	ポインタのリセット
		std::swap(m_current, m_next);
		m_next = nullptr;
	}

	//	使用するMusicを選択
	m_current = (m_current == &m_bgmA) ? &m_bgmB : &m_bgmA;

	//	前回と違うファイルなら読み込み処理をする
	if (m_currentFile != a_filename)
	{
		bool result = m_current->openFromFile(a_filename);
		if (result == true)
		{
			Logger::Info("BGMの読み込みに成功", a_filename.c_str());
		}
		else
		{
			Logger::Warn("BGMの読み込みに失敗", a_filename.c_str());
			return;
		}
	}

	//	初期化をして
	m_current->setLooping(a_loop);
	m_current->setVolume(m_volume);
	//	再生
	m_current->play();

}

/// <summary>
/// フェード機能付きBGM再生
/// </summary>
/// <param name="a_filename">切り替えたいBGMのファイル名</param>
/// <param name="a_fadeTime">フェードにかかる時間（秒）</param>
/// <param name="a_loop">true:ループ再生 false:単発再生</param>
void BGMManager::PlayBGMFade(const std::string& a_filename, float a_fadeTime, bool a_loop)
{
	//	初回再生処理
	if (m_current == nullptr)
	{
		m_current = &m_bgmA;
		auto result = m_current->openFromFile(a_filename);

		//	ログの吐き出し
		if (result == true)
		{
			Logger::Info("読み込み成功: %s", a_filename.c_str());
		}
		else
		{
			Logger::Warn("読み込み失敗: %s", a_filename.c_str());
			m_current = nullptr;
			return;
		}

		m_current->setLooping(a_loop);
		m_current->setVolume(m_volume);
		m_current->play();
	}

	//	次のBGMのポインタ取得
	m_next = (m_current == &m_bgmA) ? &m_bgmB : &m_bgmA;

	//	前回と違うファイルなら読み込みをする
	if (m_nextFile != a_filename)
	{
		auto result = m_next->openFromFile(a_filename);
		//	ログの吐き出し
		if (result == true)
		{
			Logger::Info("読み込み成功: %s", a_filename.c_str());
		}
		else
		{
			Logger::Warn("読み込み失敗: %s", a_filename.c_str());
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
/// BGMの停止（フェード機能あり）
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
/// 状態更新
/// </summary>
/// <param name="dt"></param>
void BGMManager::Update(float dt)
{
	//	フェードなし状態かBGMがないときは処理を行わない
	if (m_isFading == false || m_current == nullptr)
	{
		return;
	}
	
	//	今の音量の取得
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

	//	フェード完了処理
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
/// 音量のセット
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