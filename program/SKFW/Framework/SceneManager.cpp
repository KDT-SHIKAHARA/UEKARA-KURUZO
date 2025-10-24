
#include<SKFW/Framework/SceneManager.hpp>
#include<SKFW/Framework/Camera.h>


#include<SKFW/Ecs/GameObjectManager.hpp>
#include<SKFW/Ecs/system_RigidBody.hpp>
#include<SKFW/Ecs/system_Collision.hpp>
#include<SKFW/Ecs/system_Render.hpp>

#include<SKFW/Framework/DefaultScene.hpp>

void SceneManager::Initialize()
{
	if (m_scene == nullptr)
	{
		this->CreateScene<DefaultScene>();
	}
	else
	{
		m_scene->Initialize();
	}

}

/// <summary>
/// �ύX�V
/// </summary>
/// <param name="dt"></param>
void SceneManager::Update(float a_dt)
{
	//	�X�N���[�����Ȃ��Ƃ��͏������s��Ȃ�
	if (m_scene == nullptr)
	{
		return;
	}

	//	�I�[�o�[���C�����݂��Ă���Ƃ�
	if (m_overlay != nullptr)
	{
		m_overlay->Update(a_dt);
		
		//	�I���t���O�������Ă�����폜
		if (m_overlay->m_isFinish == true)
		{
			m_overlay.reset();
		}
	}
	else
	{
		//	�����ɏ�������ǉ����ăI�[�o�[���C������Ƃ��͍s��Ȃ��悤�ɂ���B
		GameObjectManager::Instance().Update(a_dt);
	}


}

/// <summary>
/// �Œ�X�V
/// </summary>
/// <param name="a_fixedDt"></param>
void SceneManager::FixedUpdate(float a_fixedDt)
{
	//	�X�N���[�����Ȃ��Ƃ��͏������s��Ȃ�
	if (m_scene == nullptr)
	{
		return;
	}

	if(m_overlay == nullptr)
	{
		//	�����̏�������ǉ����ăI�[�o�[���C������Ƃ��͍s��Ȃ��悤�ɂ���B
		GameObjectManager::Instance().FixedUpdate(a_fixedDt);
		RigidBodySystem::Instance().FixedUpdate(a_fixedDt);
		CollisionSystem::Instance().FixedUpdate(a_fixedDt);
	}
}

void SceneManager::Render(float a_alpha, sf::RenderWindow& a_window)
{
	//	�����̕`�揈��������
		//	���[���h
	Camera::Instance().ApplyWorldView(a_window);
	RenderSystem::Instance().WorldObjectRender(a_window, a_alpha);

	//	�X�N���[��
	Camera::Instance().ApplySceneView(a_window);
	RenderSystem::Instance().SceneObjectRender(a_window, a_alpha);

	//	�g�����W�V�����̂�����Ƃ������`��
	if (m_overlay != nullptr)
	{
		m_overlay->Render(a_window);
	}

}

/// <summary>
/// �X�N���[���̐؂�ւ�����Ɛ؂�ւ�
/// </summary>
void SceneManager::ChangeScene()
{
	if (m_pendingChange != nullptr)
	{
		m_pendingChange();
		m_pendingChange = nullptr;
	}
}
