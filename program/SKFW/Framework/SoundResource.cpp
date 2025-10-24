#include<SKFW/Framework/SoundResource.hpp>
#include<SKFW/Debug/Logger.hpp>

#include<filesystem>

sf::SoundBuffer* SoundResource::GetBuffer(const std::string& a_filePath)
{
    namespace fs = std::filesystem;
    std::string normalized = fs::relative(fs::path(a_filePath), fs::current_path()).string();

    //  ����΃R���N�V��������Q�Ƃ�Ԃ�
    auto it = m_soundBuffers.find(normalized);
    if (it != m_soundBuffers.end())
    {
        return it->second.get();
    }

    //  �Ȃ���ΐ�������
    auto buffer = std::make_unique<sf::SoundBuffer>();

    //  �Ǎ�
    auto result = buffer->loadFromFile(normalized);
    if (result == true)
    {
        Logger::Info("�T�E���h�ǂݍ��ݐ����F %s", normalized.c_str());
    }
    else
    {
        Logger::Warn("�T�E���h�ǂݍ��ݎ��s�F %s", normalized.c_str());
        return nullptr;
    }

    sf::SoundBuffer* bufPtr = buffer.get();
    m_soundBuffers[normalized] = std::move(buffer);
    return bufPtr;

}

/// <summary>
/// �w�肵���f�B���N�g����艺�̊K�w�̉����t�@�C����ǂݍ���
/// </summary>
/// <param name="a_dataRoot"></param>
void SoundResource::LoadAllFromDirectory(const std::string& a_dataRoot)
{
    namespace fs = std::filesystem;
    //  �ċA�I�Ƀt�H���_�[��T��
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
/// �w�肵���f�B���N�g���z���̃��\�[�X���폜����
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
    Logger::Info("TextureResourceManager: %s �ȉ��̃T�E���h�o�b�t�@�� %zu �폜���܂���", normalized.c_str(), before - after);

}