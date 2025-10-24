#include<SKFW/Framework/TextureResource.hpp>
#include<SKFW/Debug/Logger.hpp>

#include<filesystem>

/// <summary>
/// �f�t�H���g�̃e�N�X�`���̓ǂݍ���
/// </summary>
TextureResourceManager::TextureResourceManager()
{
}

std::string TextureResourceManager::normalizePath(const std::filesystem::path& path)
{
    std::string str = fs::relative(path, fs::current_path()).string();
    std::replace(str.begin(), str.end(), '\\', '/');
    return str;
}

/// <summary>
/// �e�N�X�`���̓ǂݍ���
/// </summary>
/// <param name="a_fimename">�t�@�C���p�X</param>
/// <returns></returns>
sf::Texture* TextureResourceManager::GetTexture(const std::string& a_fimename)
{

    fs::path path(a_fimename);

    // ��΃p�X�^���΃p�X�̍��ݖh�~
    fs::path relPath = fs::relative(path, fs::current_path());
    std::string normalized = relPath.generic_string();

    // ���łɓǂݍ��܂�Ă���ΕԂ�
    auto it = m_textures.find(normalized);
    if (it != m_textures.end())
        return it->second.get();

    // �V�K�ǂݍ���
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(normalized))
    {
        Logger::Warn("�e�N�X�`���̓ǂݍ��ݎ��s: %s", normalized.c_str());
        return nullptr;
    }

    Logger::Info("�e�N�X�`���̓ǂݍ��ݐ���: %s", normalized.c_str());

    sf::Texture* ptr = texture.get();
    m_textures[normalized] = std::move(texture);
    return ptr;
}

/// <summary>
/// �w�肵�����W�X�g�����[���K�w��.png��.jpg�������œǂݍ���
/// </summary>
/// <param name="a_dataRoot"></param>
void TextureResourceManager::LoadAllFromDirectory(const std::string& a_dataRoot)
{
    fs::path rootPath(a_dataRoot);
    if (!fs::exists(rootPath) || !fs::is_directory(rootPath))
    {
        Logger::Warn("LoadAllFromDirectory: �f�B���N�g�������݂��܂���: %s", a_dataRoot.c_str());
        return;
    }

    for (auto& entry : fs::recursive_directory_iterator(rootPath))
    {
        if (!entry.is_regular_file()) continue;

        fs::path filePath = entry.path();
        std::string ext = filePath.extension().string();

        // �g���q���������ɂ��ă`�F�b�N
        std::transform(ext.begin(), ext.end(), ext.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (ext != ".png" && ext != ".jpg" && ext != ".jpeg") continue;

        // ��΃p�X�ł͂Ȃ��A�v���O�����N���f�B���N�g������̑��΃p�X�ɓ���
        fs::path relPath = fs::relative(filePath, fs::current_path());

        // �X���b�V���𓝈� (Windows�ł� '/' ���g��)
        std::string normalized = relPath.generic_string();

        this->GetTexture(normalized);
    }
}

/// <summary>
/// �w�肵���f�B���N�g���z���̃��\�[�X���폜����
/// </summary>
/// <param name="a_dataRoot"></param>
void TextureResourceManager::UnloadAllFromDirectory(const std::string& a_dataRoot)
{
    namespace fs = std::filesystem;
    std::string normalized = fs::relative(fs::path(a_dataRoot), fs::current_path()).string();

    size_t before = m_textures.size();

    std::erase_if(m_textures,
        [&](const auto& pair)
        {
            const std::string& path = pair.first;
            return path.starts_with(normalized);
        }
    );

    size_t after = m_textures.size();
    Logger::Info("TextureResourceManager: %s �ȉ��̃e�N�X�`���� %zu �폜���܂���", normalized.c_str(), before - after);

}



