#pragma once

#include<unordered_map>
#include<string>
#include<memory>
#include<map>

#include<SKFW/Framework/Singleton.hpp>
#include<SFML/Audio/SoundBuffer.hpp>

class SoundResource : public Singleton<SoundResource>
{
	friend class Singleton<SoundResource>;
	SoundResource() = default;
	virtual ~SoundResource() = default;

public:

	/// <summary>
	/// バッファの取得
	/// まだ読まれていない場合は読み込み処理をする。
	/// </summary>
	/// <param name="a_filePath"></param>
	/// <returns></returns>
	sf::SoundBuffer* GetBuffer(const std::string& a_filePath);

	/// <summary>
	/// 指定したディレクトリ配下の音声ファイルを読み込む
	/// </summary>
	/// <param name="a_dataRoot"></param>
	void LoadAllFromDirectory(const std::string& a_dataRoot);

	/// <summary>
	/// 指定したディレクトリ配下のリソースを削除する
	/// </summary>
	/// <param name="a_dataRoot"></param>
	void UnloadAllFromDirectory(const std::string& a_dataRoot);


private:
	/// <summary>
	/// 読み込み済みのサウンドバッファのコレクション
	/// </summary>
	std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
};