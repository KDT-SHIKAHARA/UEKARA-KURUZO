#pragma once

#include<string>

class Scene
{
public:
	Scene(const std::string& a_sceneName);
	virtual ~Scene() = default;

	//	����������
	virtual void Initialize() = 0;
	virtual void Finalize();

	//	���\�[�X�̓ǂݍ��ݏ���
	void LoadResources();

	//	���\�[�X�̔p��
	void UnloadResources();

	//	�X�N���[�������擾
	std::string GetName()const noexcept;

protected:
	/// <summary>
	/// Scene�̖��O
	/// </summary>
	std::string m_name;
};