#include "system_Transform.hpp"

#include<SKFW/Debug/Logger.hpp>

/// <summary>
/// システムに登録する
/// </summary>
/// <param name="a_transform"></param>
void TransformSystem::Register(std::shared_ptr<TransformComponent> a_transform)
{
	m_transforms.push_back(a_transform);
}

/// <summary>
/// 前のフレームの状態を保存する
/// </summary>
void TransformSystem::SavePrevious()
{
	for (auto& weakTransform : m_transforms)
	{
		if (auto transform = weakTransform.lock())
		{
			transform->SavePrevious();
		}
	}
}

/// <summary>
/// 削除フラグの立っているオブジェクトを削除する。
/// この処理が呼ばれるのはフレーム末尾のみ。
/// だからインスタンスが破棄されないのを利用してフラグを見て、削除フラグが立っていたら削除をする
/// </summary>
void TransformSystem::CleanupDestroyed()
{
	std::erase_if(m_transforms,
		[](const std::weak_ptr<TransformComponent>& weakTransform)
		{
			return weakTransform.expired();
		});
}