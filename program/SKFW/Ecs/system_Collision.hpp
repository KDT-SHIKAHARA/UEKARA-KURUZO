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
	/// �Z�b�g�ɂȂ��Ă���^�O�𕪊�����
	/// </summary>
	/// <param name="a_key">�u:�v�ŋ�؂��Ă���^�O�̃Z�b�g</param>
	/// <returns>�O��ɕ��������^�O�Z�b�g</returns>
	std::pair<std::string, std::string> splitTag(const std::string& a_key);

	/// <summary>
	/// �����蔻����s���ĉ����߂������������Ȃ����ǂ����ŉ����߂����\�b�h���Ăяo��
	/// </summary>
	/// <param name="a_transform1"></param>
	/// <param name="a_collider1"></param>
	/// <param name="a_transform2"></param>
	/// <param name="a_collider2"></param>
	/// <returns></returns>
	bool checkCollision(TransformComponent* a_transform1,IColliderComponent* a_collider1, TransformComponent* a_transform2, IColliderComponent* a_collider2);

	//	�����߂�����
	void pushOut(TransformComponent* a_transform1, IColliderComponent* a_collider1, TransformComponent* a_transform2, IColliderComponent* a_collider2);

	//	�~�Ɖ~�̓����蔻��
	bool checkCircleToCircle(TransformComponent* a_transform1, CircleColliderComponent* a_collider1, TransformComponent* a_transform2, CircleColliderComponent* a_collider2);

	//	�~�Ƌ�`�̓����蔻��
	bool checkCircleToRect(TransformComponent* a_transform1, CircleColliderComponent* a_collider1, TransformComponent* a_transform2, RectColliderComponent* a_collider2);

	//	��`�Ƌ�`�̓����蔻��
	bool checkRectToRect(TransformComponent* a_transform1, RectColliderComponent* a_collider1, TransformComponent* a_transform2, RectColliderComponent* a_collider2);

	//	�O���b�h�ŋ�؂��ċ߂��̃I�u�W�F�N�g���m���������蔻������Ȃ����߂̍\����
	struct SptialHash
	{
		SptialHash(int a_cellSize = 128)
			:m_cellSize(a_cellSize)
		{ }

		/// <summary>
		/// �S�Ă̍폜
		/// </summary>
		void Clear()
		{
			m_grid.clear();
		}

		/// <summary>
		/// �Z���ɒǉ�
		/// �����Z���ɂ܂�����ꍇ�͕����L�[�ɓ������̌����B
		/// ����͒��S��ōs���B3*3�͈̔͂Ŕ��肷�邩����Ȃ�
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
						//	�ǉ�
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
		/// �Z�����̃Q�[���I�u�W�F�N�g
		/// </summary>
		std::unordered_map<long long, std::vector<GameObject*>> m_grid;
		
		/// <summary>
		/// �Z���P�ʂ̃T�C�Y
		/// </summary>
		int m_cellSize;

	}; // struct

public:
	/// <summary>
	/// �Œ�X�V�̓����蔻��
	/// </summary>
	/// <param name="fixedDt"></param>
	void FixedUpdate(float fixedDt);



private:
	/// <summary>
	/// �����蔻��̑g�ݍ��킹
	/// </summary>
	CollisionRule m_collisionRule;
};

