#include<SKFW/Framework/GameManager.hpp>
#include<SKFW/Framework/Time.hpp>
#include<SKFW/Framework/InputManager.hpp>
#include<SKFW/Debug/DebugUi.hpp>
#include<SKFW/Framework/data_window.h>
#include<SKFW/Framework/Camera.h>

#include<SKFW/Ecs/GameObjectManager.hpp>
#include<SKFW/Framework/TextureResource.hpp>
#include<SKFW/Framework/SoundResource.hpp>
#include<SKFW/Framework/SoundManager.hpp>

#include<SKFW/Ecs/system_Transform.hpp>
#include<SKFW/Ecs/system_Render.hpp>
#include<SKFW/Ecs/system_RigidBody.hpp>
#include<SKFW/Ecs/system_Collision.hpp>

#include<SKFW/Framework/SceneManager.hpp>

#include<SKFW/Debug/Logger.hpp>
#include<SKFW/Game/TitleScene.h>

#include<SKFW/Framework/TestScene.h>
#include<SKFW/Framework/TransitionOverlay.h>

GameManager::GameManager()
	:m_running(true), m_fps(240.0f), m_fixedFps(60.0f)
{
}

void GameManager::Update(float a_dt)
{
	//	Behavirour�̍X�V
	SceneManager::Instance().Update(a_dt);

	//	�T�E���h�̏�ԍX�V
	SoundManager::Instance().Update(a_dt);

	//if (InputManager::Instance().IsKeyPressed(sf::Keyboard::Key::F))
	//{
	//	SceneManager::Instance().ChangeSceneWithTransition<TestScene, TransitionOverlay>(1.0f);
	//}
}

void GameManager::FixedUpdate(float a_fixedDt)
{
	//	Behavirour�̌Œ�X�V
	//	�ړ�����
	//	�����蔻��
	SceneManager::Instance().FixedUpdate(a_fixedDt);
}

void GameManager::Render(float a_alpha)
{
	//	���[���h
	Camera::Instance().ApplyWorldView(m_window);
	RenderSystem::Instance().WorldObjectRender(m_window, a_alpha);

	//	�X�N���[��
	Camera::Instance().ApplySceneView(m_window);
	RenderSystem::Instance().SceneObjectRender(m_window, a_alpha);
}

void GameManager::HandleEvents()
{
	while (auto event = m_window.pollEvent())
	{
		DebugUI::ProcessEvent(m_window, *event);
		InputManager::Instance().ProcessEvent(*event);

		//	�I������
		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
		}
	}
}

/// <summary>
/// �ڕW��fps
/// </summary>
/// <param name="a_fps"></param>
void GameManager::SetFps(float a_fps)
{
	if (a_fps < 30)
	{
		a_fps = 30;
	}

	m_fps = a_fps;
}

/// <summary>
/// �E�B���h�E�̍쐬�A�e�V�X�e���̐����A������
/// </summary>
void GameManager::Initialize()
{
	//	���O�̏�����
	Logger::Initialize();


	//	�J�n���O���o��
	Logger::Info("�Q�[���N��");

	//	�f�[�^�ǂݍ���
	Logger::Info("�f�[�^�ǂݍ��݊J�n");
	TextureResourceManager::Instance().LoadAllFromDirectory("assets/textures/");
	SoundResource::Instance().LoadAllFromDirectory("assets/sounds/");

	//SoundResource::Instance().UnloadAllFromDirectory("assets/sounds/");

	//	�E�B���h�E������
	//	�K�v��������E�B���h�E�p��config�t�@�C����ǉ����ăf�[�^���擾���Ă���쐬���Ă�����
	{
		Logger::Info("�E�B���h�E�̍쐬");
		sf::State mode = sf::State::Windowed;
		m_window = sf::RenderWindow(WindowData::m_video, WindowData::m_windowName, sf::Style::Default | sf::Style::Resize, WindowData::m_mode);
		m_window.setFramerateLimit(m_fps);
		DebugUI::Initialize(m_window);
	}

	//	���̎��_�ōŏ��̃V�[���𐶐����Ă���
	SceneManager::Instance().CreateScene<TitleScene>();

	//	������
	SceneManager::Instance().Initialize();

	//	���ԊǗ�������
	Time::Instance().Initialize(m_fixedFps);

	//	�J�����̏�����
	Camera::Instance().Initialize(m_window.getSize());

	//	�J�n���O���o��
	Logger::Info("�Q�[���J�n���̃t���[�����[�g�ݒ� FPS: %.2f, Fixed FPS: %.2f", m_fps, m_fixedFps);

}

/// <summary>
/// �Q�[�����[�v�A�e�V�X�e���̏�ԍX�V�A�`��X�V���s���B
/// </summary>
void GameManager::Run()
{
	auto& time = Time::Instance();
	Logger::Info("�Q�[�����[�v�̊J�n");

	//	���[�v
	while (m_window.isOpen() && m_running == true)
	{
		//	�E�B���h�E�̃C�x���g�֌W
		HandleEvents();

		//	��Ƀt���[���J�n�n�_�Ŏ��Ԍo�߂��Ȃ��Ƃ����Ȃ��̂�
		time.Update();


		//	ImGui�̓�����ԍX�V
		DebugUI::Update(m_window, sf::seconds(time.DeltaTime()));


		//	�ύX�V
		this->Update(time.DeltaTime());

		while (time.ShouldFixedUpdate())
		{

			//	���W�n�̑S�̂̏�Ԃ�ۑ�
			TransformSystem::Instance().SavePrevious();

			//	�Œ�X�V
			this->FixedUpdate(time.FixedDelta());
	
			//	�Œ�X�V�̎��Ԍo��
			time.ConsumeFixedDelta();
		}

		Time::Instance().UpdateAlpha();
		
		m_window.clear(sf::Color(64,64,64));

		//	��Ԃ�n���Ċ��炩�ɂ��ĕ`�悷��
		SceneManager::Instance().Render(time.Alpha(), m_window);

		//	ImGui�`��
		DebugUI::Instance().Draw();
		DebugUI::Render(m_window);

		m_window.display();

		InputManager::Instance().Update();

		//	�X�N���[���̐؂�ւ�
		SceneManager::Instance().ChangeScene();

		//	�폜���[�v
		GameObjectManager::Instance().CleanupDestroyedObjects();
		TransformSystem::Instance().CleanupDestroyed();
		RenderSystem::Instance().ProcessExpired();
		RigidBodySystem::Instance().CleanupDestroyed();
	}

	Logger::Info("�Q�[�����[�v�̏I��");


}

/// <summary>
/// �I�������A�E�B���h�E�̔j���A�t���[�����[�N�̍폜���䂪�K�v�Ȃ�s��
/// </summary>
void GameManager::Finalize()
{
	DebugUI::Shutdown();
	Logger::Info("�Q�[���̐���I��");
}

/// <summary>
/// �Q�[�����[�v�̏I��
/// </summary>
void GameManager::Stop()
{
	m_running = false;
}
