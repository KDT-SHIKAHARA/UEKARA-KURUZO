#pragma once
#include<SKFW/Framework/ObjectPool.hpp>
#include<SKFW/Ecs/comp_Transform.hpp>
#include<SKFW/Framework/Singleton.hpp>

#include<vector>


class TransformSystem : public Singleton<TransformSystem>
{
	friend class Singleton<TransformSystem>;
	TransformSystem() = default;

public:

	/// <summary>
	/// システムに登録する
	/// </summary>
	/// <param name="a_transform"></param>
	void Register(std::shared_ptr<TransformComponent> a_transform);

	/// <summary>
	/// 前のフレームの状態を保存する
	/// </summary>
	void SavePrevious();

	/// <summary>
	/// 削除フラグの立っているオブジェクトを削除する。
	/// この処理が呼ばれるのはフレーム末尾のみ。
	/// だからインスタンスが破棄されないのを利用してフラグを見て、削除フラグが立っていたら削除をする
	/// </summary>
	void CleanupDestroyed();

private:
	/// <summary>
	/// 今アクティブなtランスフォームの参照
	/// </summary>
	std::vector<std::weak_ptr<TransformComponent>> m_transforms;

};