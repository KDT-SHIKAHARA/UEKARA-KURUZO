#pragma once
#include<SKFW/Debug/DebugTab.hpp>
#include<SKFW/Ecs/GameObjectManager.hpp>

#include<SKFW/Ecs/comp_Transform.hpp>
#include<SKFW/Ecs/comp_Image.hpp>
#include<SKFW/Framework/TextureResource.hpp>
#include<SKFW/Ecs/comp_RigidBody.hpp>
#include<SKFW/Ecs/comp_Animation.hpp>
#include<SKFW/Ecs/comp_collider.hpp>

#include<SKFW/Ecs/comp_DrawCollider.h>

#include<SKFW/Framework/Camera.h>

#include<SKFW/Framework/SoundManager.hpp>

class DebugGameObjectManager : public DebugTabBase
{
public:
	DebugGameObjectManager()
		:DebugTabBase("GameObjects")
	{ }

	void Draw()override
	{
		ImGui::Text("GameObjects:");
		ImGui::Separator();

		ImGui::Text("Active Count: %5d", GameObjectManager::Instance().GetActiveCount());

		ImGui::Separator();

		//	プレイヤー追加ボタン
		if (ImGui::Button("Player Add"))
		{

			for (int i = 0; i < 1; i++)
			{
				auto& obj = GameObjectManager::Instance().Create(eObjectLivedType::Long, "Player");
				auto trans = obj->AddComponent<TransformComponent>();
				trans->SetPosition({ 600,000 });
				//auto texture = TextureResourceManager::Instance().GetTexture(DEFAULT_TEXTURE_PATH);
				//obj->AddComponent<ImageComponent>(texture, 0);
				auto rigid = obj->AddComponent<RigidBodyComponent>();

				auto anim = obj->AddComponent<AnimationComponent>(0);
				anim->AddFrameAnimation("Idle", "assets/textures/blue_slime/", 0, 3, IAnimation2D::eMode::Loop, IAnimation2D::eType::Frame, 0.2f);

				auto collider = obj->AddComponent<RectColliderComponent>();
				obj->AddComponent<DrawRectCollider>(trans.get(), collider.get());

			}



			auto& obj2 = GameObjectManager::Instance().Create(eObjectLivedType::Long, "Enemy");
			auto trans2 = obj2->AddComponent<TransformComponent>();
			trans2->SetPosition({ 600,100 });
			auto anim2 = obj2->AddComponent<AnimationComponent>(0);
			anim2->AddFrameAnimation("Idle", "assets/textures/blue_slime/", 0, 3, IAnimation2D::eMode::Loop, IAnimation2D::eType::Frame, 0.2f);

			auto collider2 = obj2->AddComponent<RectColliderComponent>();
			collider2->m_isCollisioned.Set(Flag::Off);
			obj2->AddComponent<DrawRectCollider>(trans2.get(), collider2.get());
			Camera::Instance().SetZoom(1.0f);
			Camera::Instance().SetCenter(trans2->GetPosition());
			SoundManager::Instance().SE().Play("assets/sounds/se.wav");
		}

	}
};
