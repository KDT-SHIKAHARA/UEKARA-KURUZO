#pragma once
#include<string>
#include<set>

class CollisionRule
{
	std::string MakepairKey(const std::string& a_tag1, const std::string& a_tag2)const
	{
		return (a_tag1 < a_tag2) ? a_tag1 + ":" + a_tag2 : a_tag2 + ":" + a_tag1;
	}
public:
	CollisionRule() = default;
	virtual ~CollisionRule() = default;

	//	�g�ݍ��킹�̒ǉ�
	void AddRule(const std::string& a_tag1, const std::string& a_tag2) {
		m_pair.insert(MakepairKey(a_tag1, a_tag2));
	}

	//	�����蔻����s�����ǂ����̔���
	//	true; �s�� 
	//	false: �s��Ȃ�
	bool CanCollide(const std::string& a_tag1, const std::string& a_tag2)const {
		return m_pair.contains(MakepairKey(a_tag1, a_tag2));
	}

	//	�S�Ă̑g�ݍ��킹�擾
	const std::set<std::string>& GetAllRules()const {
		return m_pair;
	}

private:
	std::set<std::string> m_pair;

};