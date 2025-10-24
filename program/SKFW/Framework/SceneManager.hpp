#pragma once
#include<SKFW/Framework/Scene.hpp>
#include<SKFW/Framework/Singleton.hpp>
#include<SKFW/Debug/Logger.hpp>
#include<SKFW/Framework/base_IOverlay.hpp>

#include<memory>
#include<functional>

class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	virtual ~SceneManager() = default;
public:

	/// <summary>
	/// �������A�����V�[���̃|�C���^�̒��g���Ȃ�������f�t�H���g�𐶐�����
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�N���[���̐����i�g�����W�V�����Ȃ��j
	/// </summary>
	template<typename T>
	void CreateScene();

	/// <summary>
	/// �X�N���[���̐؂�ւ��i�g�����W�V�����Ȃ��j
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	void ChangeSceneRequest();

	/// <summary>
	/// �g�����W�V�����t����ʐ؂�ւ�
	/// </summary>
	/// <typeparam name="Scene"></typeparam>
	/// <typeparam name="Transition"></typeparam>
	template<typename NEXT,typename Transition>
	void ChangeSceneWithTransition(float a_fadeTime);

	/// <summary>
	/// �ύX�V
	/// </summary>
	/// <param name="dt"></param>
	void Update(float a_dt);

	/// <summary>
	/// �Œ�X�V
	/// </summary>
	/// <param name="a_fixedDt"></param>
	void FixedUpdate(float a_fixedDt);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="a_alpha"></param>
	void Render(float a_alpha,sf::RenderWindow& a_window);

	/// <summary>
	/// �X�N���[���̐؂�ւ�����Ɛ؂�ւ�
	/// </summary>
	void ChangeScene();

private:
	/// <summary>
	/// ���̃X�N���[���̃|�C���^
	/// </summary>
	std::unique_ptr<Scene> m_scene;

	/// <summary>
	/// �I�[�o�[���C�̃|�C���^
	/// </summary>
	std::unique_ptr<OverlayScene> m_overlay;

	/// <summary>
	/// �t���[�����ŃX�N���[����ύX����B
	/// �r���ŕύX����ƎQ�ƃG���[�Ȃ�̂�
	/// </summary>
	std::function<void()> m_pendingChange;
};

/// <summary>
/// �X�N���[���̐����i�g�����W�V�����Ȃ��j
/// </summary>
template<typename T>
inline void SceneManager::CreateScene()
{
	if (std::is_base_of_v<Scene, T> == false)
	{
		Logger::Error("Scene���p�����Ă��Ȃ��^���������ꂻ���ɂȂ�܂���");
		return;
	}
	m_scene = std::make_unique<T>();
	Logger::Info("Scene: %s ����������܂���", m_scene->GetName().c_str());
}

template<typename T>
inline void SceneManager::ChangeSceneRequest()
{
	if (std::is_base_of_v<Scene, T> == false)
	{
		Logger::Error("Scene���p�����Ă��Ȃ��^���������ꂻ���ɂȂ�܂���");
		return;
	}
	
	//	���̃X�N���[���̏I���������Ăяo��
	//	GameObject�̕ԋp�Ƃ�
	if (m_scene != nullptr)
	{
		m_scene->Finalize();
	}

	m_pendingChange = [this]()
	{
		auto scene = std::make_unique<T>();
		scene->Initialize();
		m_scene = std::move(scene);
	};
}

template<typename NEXT, typename Transition>
inline void SceneManager::ChangeSceneWithTransition(float a_fadeTime)
{
	if (std::is_base_of_v<Scene,NEXT> == false)
	{
		Logger::Error("Scene���p�����Ă��Ȃ��^���������ꂻ���ɂȂ�܂���");
		return;
	}

	if (std::is_base_of_v<OverlayScene, Transition> == false)
	{
		Logger::Error("OverlayScene���p�����Ă��Ȃ��^���������ꂻ���ɂȂ�܂���");
		return;
	}

	auto scheduleChange = [this]()
	{
			m_pendingChange = [this]()
			{
					m_scene->Finalize();
					auto scene = std::make_unique<NEXT>();
					scene->Initialize();
					m_scene = std::move(scene);
					Logger::Info("Scene: %s ����������܂���", m_scene->GetName().c_str());
			};
	};

	m_overlay = std::make_unique<Transition>(scheduleChange, a_fadeTime);
	m_overlay->Initialize();
	Logger::Info("�g�����W�V�����̐���");

}
