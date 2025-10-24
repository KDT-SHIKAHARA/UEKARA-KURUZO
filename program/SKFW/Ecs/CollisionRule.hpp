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

	//	組み合わせの追加
	void AddRule(const std::string& a_tag1, const std::string& a_tag2) {
		m_pair.insert(MakepairKey(a_tag1, a_tag2));
	}

	//	当たり判定を行うかどうかの判定
	//	true; 行う 
	//	false: 行わない
	bool CanCollide(const std::string& a_tag1, const std::string& a_tag2)const {
		return m_pair.contains(MakepairKey(a_tag1, a_tag2));
	}

	//	全ての組み合わせ取得
	const std::set<std::string>& GetAllRules()const {
		return m_pair;
	}

private:
	std::set<std::string> m_pair;

};