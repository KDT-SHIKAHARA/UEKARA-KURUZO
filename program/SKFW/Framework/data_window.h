#pragma once

#include<SFML/Window/VideoMode.hpp>
#include<SFML/Window/WindowEnums.hpp>
#include<SFML/Graphics/Color.hpp>

#include<string>


class WindowData
{
public:
	static inline sf::VideoMode m_video = sf::VideoMode({ 1280,720 });
	static inline std::string m_windowName = "SKFW";
	static inline sf::State m_mode = sf::State::Windowed;
private:
};