#pragma once
#include<SKFW/Framework/Singleton.hpp>
#include<SKFW/Ecs/CollisionRule.hpp>
#include<SKFW/Ecs/comp_collider.hpp>
#include<SKFW/Ecs/comp_Transform.hpp>

#include<string>
#include<vector>
#include<unordered_map>


class CollisionSystem : public Singleton<CollisionSystem>
{
	friend class Singleton<CollisionSystem>;
	CollisionSystem();
	virtual ~CollisionSystem() = default;

	/// <summary>
	/// セットになっているタグを分割する
	/// </summary>
	/// <param name="a_key">「:」で区切られているタグのセット</param>
	/// <returns>前後に分割したタグセット</returns>
	std::pair<std::string, std::string> splitTag(const std::string& a_key);

	/// <summary>
	/// 当たり判定を行って押し戻し処理をおこなうかどうかで押し戻しメソッドを呼び出す
	/// </summary>
	/// <param name="a_transform1"></param>
	/// <param name="a_collider1"></param>
	/// <param name="a_transform2"></param>
	/// <param name="a_collider2"></param>
	/// <returns></returns>
	bool checkCollision(TransformComponent* a_transform1,IColliderComponent* a_collider1, TransformComponent* a_transform2, IColliderComponent* a_collider2);

	//	押し戻し処理
	void pushOut(TransformComponent* a_transform1, IColliderComponent* a_collider1, TransformComponent* a_transform2, IColliderComponent* a_collider2);

	//	円と円の当たり判定
	bool checkCircleToCircle(TransformComponent* a_transform1, CircleColliderComponent* a_collider1, TransformComponent* a_transform2, CircleColliderComponent* a_collider2);

	//	円と矩形の当たり判定
	bool checkCircleToRect(TransformComponent* a_transform1, CircleColliderComponent* a_collider1, TransformComponent* a_transform2, RectColliderComponent* a_collider2);

	//	矩形と矩形の当たり判定
	bool checkRectToRect(TransformComponent* a_transform1, RectColliderComponent* a_collider1, TransformComponent* a_transform2, RectColliderComponent* a_collider2);

	//	グリッドで区切って近くのオブジェクト同士しか当たり判定をしないための構造体
	struct SptialHash
	{
		SptialHash(int a_cellSize = 128)
			:m_cellSize(a_cellSize)
		{ }

		/// <summary>
		/// 全ての削除
		/// </summary>
		void Clear()
		{
			m_grid.clear();
		}

		/// <summary>
		/// セルに追加
		/// 複数セルにまたがる場合は複数キーに入れるもの検討。
		/// 現状は中心基準で行う。3*3の範囲で判定するから問題なし
		/// </summary>
		/// <param name="a_obj"></param>
		/// <param name="a_position"></param>
		void Insert(GameObject* a_obj, const sf::Vector2f& a_position)
		{
			int cx = static_cast<int>(std::floor(a_position.x / m_cellSize));
			int cy = static_cast<int>(std::floor(a_position.y / m_cellSize));
			long long key = this->Hash(cx, cy);
			m_grid[key].push_back(a_obj);
		}

		std::vector<GameObject*> QueryNearby(const sf::Vector2f& a_position) const
		{
			std::vector<GameObject*> out;
			int cx = static_cast<int>(std::floor(a_position.x / m_cellSize));
			int cy = static_cast<int>(std::floor(a_position.y / m_cellSize));

			for (int dy = -1; dy <= 1; dy++)
			{
				for (int dx = -1; dx <= 1; dx++)
				{
					long long k = this->Hash(cx + dx, cy + dy);
					auto it = m_grid.find(k);
					if (it != m_grid.end())
					{
						//	追加
						out.insert(out.end(), it->second.begin(), it->second.end());
					}
				}
			}
			return out;

		}

		long long Hash(int a_cellX, int a_cellY)const
		{
			return (static_cast<long long>(a_cellX) << 32) ^ static_cast<unsigned long long>(a_cellY);
		}

		/// <summary>
		/// セル内のゲームオブジェクト
		/// </summary>
		std::unordered_map<long long, std::vector<GameObject*>> m_grid;
		
		/// <summary>
		/// セル単位のサイズ
		/// </summary>
		int m_cellSize;

	}; // struct

public:
	/// <summary>
	/// 固定更新の当たり判定
	/// </summary>
	/// <param name="fixedDt"></param>
	void FixedUpdate(float fixedDt);



private:
	/// <summary>
	/// 当たり判定の組み合わせ
	/// </summary>
	CollisionRule m_collisionRule;
};

