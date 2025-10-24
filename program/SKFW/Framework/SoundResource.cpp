#include<SKFW/Framework/SoundResource.hpp>
#include<SKFW/Debug/Logger.hpp>

#include<filesystem>

sf::SoundBuffer* SoundResource::GetBuffer(const std::string& a_filePath)
{
    namespace fs = std::filesystem;
    std::string normalized = fs::relative(fs::path(a_filePath), fs::current_path()).string();

    //  あればコレクションから参照を返す
    auto it = m_soundBuffers.find(normalized);
    if (it != m_soundBuffers.end())
    {
        return it->second.get();
    }

    //  なければ生成する
    auto buffer = std::make_unique<sf::SoundBuffer>();

    //  読込
    auto result = buffer->loadFromFile(normalized);
    if (result == true)
    {
        Logger::Info("サウンド読み込み成功： %s", normalized.c_str());
    }
    else
    {
        Logger::Warn("サウンド読み込み失敗： %s", normalized.c_str());
        return nullptr;
    }

    sf::SoundBuffer* bufPtr = buffer.get();
    m_soundBuffers[normalized] = std::move(buffer);
    return bufPtr;

}

/// <summary>
/// 指定したディレクトリより下の階層の音声ファイルを読み込む
/// </summary>
/// <param name="a_dataRoot"></param>
void SoundResource::LoadAllFromDirectory(const std::string& a_dataRoot)
{
    namespace fs = std::filesystem;
    //  再帰的にフォルダーを探索
    for (auto& entry : std::filesystem::recursive_directory_iterator(a_dataRoot))
    {
        if (entry.is_regular_file())
        {
            auto path = entry.path().string();
            if (path.ends_with(".wav") || path.ends_with(".ogg") || path.ends_with(".flac"))
            {
                std::string relPath = fs::relative(entry.path(), fs::current_path()).string();
                this->GetBuffer(relPath);
            }
        }
    }
}

/// <summary>
/// 指定したディレクトリ配下のリソースを削除する
/// </summary>
/// <param name="a_dataRoot"></param>
void SoundResource::UnloadAllFromDirectory(const std::string& a_dataRoot)
{
    namespace fs = std::filesystem;
    std::string normalized = fs::relative(fs::path(a_dataRoot), fs::current_path()).string();

    size_t before = m_soundBuffers.size();

    std::erase_if(m_soundBuffers,
        [&](const auto& pair)
        {
            const std::string& path = pair.first;
            return path.starts_with(normalized);
        }
    );
    size_t after = m_soundBuffers.size();
    Logger::Info("TextureResourceManager: %s 以下のサウンドバッファを %zu 個削除しました", normalized.c_str(), before - after);

}