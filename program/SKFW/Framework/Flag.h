#pragma once

class Flag {
	bool m_flag;
public:
	enum {
		On = true,
		Off = false
	};

	Flag(bool a_flag = false) :m_flag(a_flag) {}
	void Toggle() { m_flag = !m_flag; }
	void Set(bool a_flag) { m_flag = a_flag; }
	operator bool()const { return m_flag; }
};