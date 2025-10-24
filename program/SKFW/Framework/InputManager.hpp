#pragma once
#include<unordered_map>

#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<SKFW/Framework/Singleton.hpp>
#include<imgui.h>

//	�L�[�̏��
enum KeyState {
	none,
	pressed,
	held,
	released,
};

// Xbox�{�^��
enum class XboxButton {
	A = 0,
	B = 1,
	X = 2,
	Y = 3,
	LB = 4,
	RB = 5,
	Back = 6,
	Start = 7,
	LS = 8,
	RS = 9
};

// PS�{�^��
enum class PSButton {
	Cross = 0,   // A�ɑ���
	Circle = 1,  // B�ɑ���
	Square = 2,  // X�ɑ���
	Triangle = 3,// Y�ɑ���
	L1 = 4,
	R1 = 5,
	Share = 6,
	Options = 7,
	L3 = 8,
	R3 = 9
};

// Switch Pro�{�^��
enum class SwitchButton {
	B = 0,    // A�ɑ���
	A = 1,    // B�ɑ���
	Y = 2,    // X�ɑ���
	X = 3,    // Y�ɑ���
	L = 4,
	R = 5,
	Minus = 6,
	Plus = 7,
	LStick = 8,
	RStick = 9
};


/// <summary>
/// ���͊Ǘ��N���X�Asfml������͏�Ԃ������Ă��ăt���[�����Ƃɍs���B
/// </summary>
class InputManager : public Singleton<InputManager> {
	//	�}�E�X�̓��͗p
	struct MouseState {
		sf::Vector2i position;
		KeyState left = KeyState::none;
		KeyState right = KeyState::none;
	};

	//	PAD�̓��͗p
	struct PadState
	{
		std::unordered_map<unsigned int, KeyState> buttons;	//	�{�^�����Ƃ̏��
		sf::Vector2f axisLeft{0.0f,0.0f};	//	���X�e�B�b�N
		sf::Vector2f axisRight{ 0.0f,0.0f };	//	�E�X�e�B�b�N
		sf::Vector2f dpad{ 0.0f, 0.0f };       // �\���L�[�̎�
		float triggerLeft = 0.0f;	//	Xbox�p�̍��g���K�[�擾
		float triggerRight = 0.0f;	//	Xbox�p�̉E�g���K�[�擾
		float deadZoneLeft = 0.2f;
		float deadZoneRight = 0.2f;
		bool connected = false;	//	�ڑ�����
	};

	//	�L�[�̓��͏�Ԏ擾
	KeyState GetKeyState(sf::Keyboard::Key key)const {
		auto it = m_keyStates.find(key);
		if (it != m_keyStates.end()) return it->second;
		return KeyState::none;
	}

	//	Pad�̓��͏�Ԃ̍X�V
	void UpdatePads()
	{
		//	�h�����Ă���PAD�̐���
		for (unsigned int id = 0; id < sf::Joystick::Count; id++)
		{
			//	�ڑ�����
			PadState& pad = m_pads[id];
			pad.connected = sf::Joystick::isConnected(id);
			if (pad.connected == false)
			{
				continue;
			}

			//	�{�^���̍X�V
			for (unsigned int button = 0; button < sf::Joystick::getButtonCount(id); button++)
			{
				//	������Ă��邩����
				bool pressed = sf::Joystick::isButtonPressed(id, button);
				auto& state = pad.buttons[button];
				//	������Ă����ꍇ�i����������p�j
				if (pressed == true)
				{
					//	�c�t���[��������Ă������ǂ���
					if (state != KeyState::held)
					{
						state = KeyState::pressed;
					}
				}
				//	������Ă��Ȃ����̑O�t���[���̏�Ԃŗ������u�Ԃ��ǂ���
				else
				{
					//	�O�t���[��������Ă�����
					if (state == KeyState::held || state == KeyState::pressed)
					{
						state = KeyState::released;
					}
					//	�Ȃ��Ƃ��͉����Ȃ�
					else
					{
						state = KeyState::none;
					}
				}
			}

			// ���̌X���擾���� -1.0f~1.0f�ɃN�����v
			pad.axisLeft.x = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::X) / 100.f;
			pad.axisLeft.y = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::Y) / 100.f;
			
			
			std::string name = sf::Joystick::getIdentification(id).name;

			if (name.find("Wireless Controller") != std::string::npos) // DualShock4
			{
				pad.axisRight.x = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::Z) / 100.f;
				pad.axisRight.y = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::R) / 100.f;
			}
			else if (name.find("Xbox") != std::string::npos ||
				name.find("Controller (XBOX") != std::string::npos) {
				pad.axisRight.x = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::U) / 100.f;
				pad.axisRight.y = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::V) / 100.f;

				// �g���K�[�l�̍X�V�i���������Ή��j
				float z = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::Z);
				float lt = 0.f, rt = 0.f;

				// Z���� -100 �` +100 �͈̔͂����L���Ă���
				if (z < 0)
					lt = -z / 100.f; // LT�������ƕ�����
				else if (z > 0)
					rt = z / 100.f;  // RT�������Ɛ�����

				pad.triggerLeft = std::clamp(lt, 0.f, 1.f);
				pad.triggerRight = std::clamp(rt, 0.f, 1.f);
			}
			else // �s���Ȃ�
			{
				pad.axisRight.x = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::U) / 100.f;
				pad.axisRight.y = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::V) / 100.f;
			}

			//pad.axisRight.x = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::Z) / 100.f;
			//pad.axisRight.y = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::R) / 100.f;


			// �N�����v���Ă����ƈ��S
			pad.axisLeft.x = std::clamp(pad.axisLeft.x, -1.f, 1.f);
			pad.axisLeft.y = std::clamp(pad.axisLeft.y, -1.f, 1.f);
			pad.axisRight.x = std::clamp(pad.axisRight.x, -1.f, 1.f);
			pad.axisRight.y = std::clamp(pad.axisRight.y, -1.f, 1.f);

			pad.axisLeft = ApplyDeadZone(pad.axisLeft, pad.deadZoneLeft);
			pad.axisRight = ApplyDeadZone(pad.axisRight, pad.deadZoneRight);


			// --- D-Pad / �\���L�[ ---
			float dpadX = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::PovX) / 100.f;
			float dpadY = -sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::PovY) / 100.f; // �オ���Ȃ̂Ŕ��]

			// �f�b�h�]�[���␳�i�y���j
			if (std::abs(dpadX) < 0.1f) dpadX = 0.f;
			if (std::abs(dpadY) < 0.1f) dpadY = 0.f;

			pad.dpad = { dpadX, dpadY };
		}
	}

	//	�f�b�h�]�[����K������
	sf::Vector2f ApplyDeadZone(const sf::Vector2f& input, float deadZone = 0.2f) {
		sf::Vector2f result = input;

		// ���͂̑傫��
		float len = std::sqrt(input.x * input.x + input.y * input.y);

		if (len < deadZone) {
			result = { 0.f, 0.f };  // �f�b�h�]�[�����Ȃ�[��
		}
		else {
			// �f�b�h�]�[���O��0?1�ɍĐ��K��
			float factor = (len - deadZone) / (1.f - deadZone);
			result = input / len * factor;
		}

		return result;
	}

	// �}�b�v�p�ɖ��O���o��
	static std::string AxisName(sf::Joystick::Axis axis) {
		switch (axis) {
		case sf::Joystick::Axis::X: return "X";
		case sf::Joystick::Axis::Y: return "Y";
		case sf::Joystick::Axis::Z: return "Z";
		case sf::Joystick::Axis::R: return "R";
		case sf::Joystick::Axis::U: return "U";
		case sf::Joystick::Axis::V: return "V";
		case sf::Joystick::Axis::PovX: return "POV X";
		case sf::Joystick::Axis::PovY: return "POV Y";
		default: return "Unknown";
		}
	}

	static float NormalizeAxis(float raw100) {
		// SFML �� -100 .. 100 ��Ԃ��̂� -1..1 ��
		return raw100 / 100.0f;
	}


public:

	//	�t���[������
	void Update() {
		//	�L�[��Ԃ𒷉����������Ă����ԂɕύX
		for (auto& [key, state] : m_keyStates) {
			if (state == KeyState::pressed) state = KeyState::held;
			else if (state == KeyState::released) state = KeyState::none;
		}

		//	��
		if (m_mouse.left == KeyState::pressed) m_mouse.left = KeyState::held;
		else if (m_mouse.left == KeyState::released) m_mouse.left = KeyState::none;

		//	�E
		if (m_mouse.right == KeyState::pressed) m_mouse.right = KeyState::held;
		if (m_mouse.right == KeyState::released) m_mouse.right = KeyState::none;

		// pad�̓��͍X�V
		UpdatePads();
	}

	//	���͏�Ԃ̍X�V
	void ProcessEvent(const sf::Event& event) {
		//	==========�L�[�{�[�h==========
		//	����
		if (event.is<sf::Event::KeyPressed>()) {
			const auto* key = event.getIf<sf::Event::KeyPressed>();
			if (key) {
				auto& state = m_keyStates[key->code];
				if (state != KeyState::held) {
					state = KeyState::pressed;
				}
			}
		}
		//	����
		else if (event.is<sf::Event::KeyReleased>()) {
			const auto* key = event.getIf<sf::Event::KeyReleased>();
			if (key) m_keyStates[key->code] = KeyState::released;
		}

		//	==========�}�E�X==========
		//	����
		else if (event.is<sf::Event::MouseButtonPressed>()) {
			const auto* m = event.getIf<sf::Event::MouseButtonPressed>();
			if (m) {
				if (m->button == sf::Mouse::Button::Left) m_mouse.left = KeyState::pressed;
				else if (m->button == sf::Mouse::Button::Right) m_mouse.right = KeyState::pressed;
			}
		}
		//	����
		else if (event.is<sf::Event::MouseButtonReleased>()) {
			const auto* m = event.getIf<sf::Event::MouseButtonReleased>();
			if (m) {
				if (m->button == sf::Mouse::Button::Left) m_mouse.left = KeyState::released;
				else if (m->button == sf::Mouse::Button::Right)m_mouse.right = KeyState::released;
			}
		}
		//	�}�E�X�ړ�
		else if (event.is<sf::Event::MouseMoved>()) {
			const auto* m = event.getIf<sf::Event::MouseMoved>();
			if (m) m_mouse.position = m->position;
		}

	}

	//	-----�L�[����----- //
	//	�������u�Ԃ���true
	bool IsKeyPressed(sf::Keyboard::Key key)const {
		return GetKeyState(key) == KeyState::pressed;
	}

	//	�����Ă����true
	bool IsKeyHeld(sf::Keyboard::Key key)const {
		return GetKeyState(key) == KeyState::held;
	}

	//	�������u��
	bool IsKeyReleased(sf::Keyboard::Key key)const {
		return GetKeyState(key) == KeyState::released;
	}

	//	�ǂꂩ�̃L�[��������Ă�����true
	bool IsAnyKeyDown()const {
		for (auto& [_, state] : m_keyStates) {
			if (state == KeyState::pressed || state == KeyState::held) return true;
		}
		return false;
	}

	//	-----�}�E�X----- //
	//	�������u�Ԃ���true
	bool IsMousePressed(sf::Mouse::Button btn)const {
		if (btn == sf::Mouse::Button::Left) return m_mouse.left == KeyState::pressed;
		if (btn == sf::Mouse::Button::Right) return m_mouse.right == KeyState::pressed;
		return false;
	}

	//	�����Ă����true
	bool IsMouseHeld(sf::Mouse::Button btn) const {
		if (btn == sf::Mouse::Button::Left) return m_mouse.left == KeyState::held;
		if (btn == sf::Mouse::Button::Right) return m_mouse.right == KeyState::held;
		return false;
	}

	//	�������u��true
	bool IsMouseReleased(sf::Mouse::Button btn) const {
		if (btn == sf::Mouse::Button::Left) return m_mouse.left == KeyState::released;
		if (btn == sf::Mouse::Button::Right) return m_mouse.right == KeyState::released;
		return false;
	}

	//	���W
	const sf::Vector2i& GetMousePosition() const { return m_mouse.position; }

	// ----- �p�b�h���� -----
	bool IsPadConnected(unsigned int id) const {
		if (id >= m_pads.size()) return false;
		return m_pads[id].connected;
	}

	bool IsPadPressed(unsigned int id, unsigned int button) const {
		if (id >= m_pads.size()) return false;
		auto it = m_pads[id].buttons.find(button);
		return it != m_pads[id].buttons.end() && it->second == KeyState::pressed;
	}
	bool IsPadHeld(unsigned int id, unsigned int button) const {
		if (id >= m_pads.size()) return false;
		auto it = m_pads[id].buttons.find(button);
		return it != m_pads[id].buttons.end() && it->second == KeyState::held;
	}
	bool IsPadReleased(unsigned int id, unsigned int button) const {
		if (id >= m_pads.size()) return false;
		auto it = m_pads[id].buttons.find(button);
		return it != m_pads[id].buttons.end() && it->second == KeyState::released;
	}

	sf::Vector2f GetPadAxisLeft(unsigned int id) const {
		if (id >= m_pads.size()) return { 0.f,0.f };
		return m_pads[id].axisLeft;
	}
	sf::Vector2f GetPadAxisRight(unsigned int id) const {
		if (id >= m_pads.size()) return { 0.f,0.f };
		return m_pads[id].axisRight;
	}

	//	�\���L�[�̎��̎擾
	sf::Vector2f GetPadDPad(unsigned int id) const {
		if (id >= m_pads.size()) return { 0.f, 0.f };
		return m_pads[id].dpad;
	}

// �g���K�[��������Ă��邩�i���ȏ�j
bool IsPadTriggerLeft(unsigned int id) const {
    if (id >= m_pads.size()) return false;
    return m_pads[id].triggerLeft > 0.2f; // 20%�ȏ㉟���ꂽ��true
}

bool IsPadTriggerRight(unsigned int id) const {
    if (id >= m_pads.size()) return false;
    return m_pads[id].triggerRight > 0.2f;
}


	//	�f�o�b�N�\��
	void ShowPadDebug(unsigned int id) {
		if (!sf::Joystick::isConnected(id)) {
			ImGui::Text("Pad %u: (disconnected)", id);
			return;
		}

		ImGui::Separator();
		ImGui::Text("Pad %u", id);

		std::string name = sf::Joystick::getIdentification(id).name;
		ImGui::Text("Controller Name: %s", name.c_str());

		// --- Buttons ---
		unsigned int btnCount = sf::Joystick::getButtonCount(id);
		ImGui::Text("Buttons: %u", btnCount);

		ImGui::BeginChild(("buttons_child_" + std::to_string(id)).c_str(), ImVec2(0, 0), true);
		const int cols = 8;
		ImGui::Columns(cols, nullptr, false);
		for (unsigned int b = 0; b < btnCount; ++b) {
			bool pressed = sf::Joystick::isButtonPressed(id, b);
			if (pressed)
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
			ImGui::Text("B%u", b);
			if (pressed)
				ImGui::PopStyleColor();
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::EndChild();

		// --- Axes ---
		auto& pad = m_pads[id];
		sf::Vector2f left = pad.axisLeft;
		sf::Vector2f right = pad.axisRight;

		ImGui::Spacing();
		ImGui::Text("Left Stick (%.2f, %.2f)", left.x, left.y);
		ImGui::ProgressBar((left.x + 1.0f) / 2.0f, ImVec2(100, 8), "X");
		ImGui::ProgressBar((left.y + 1.0f) / 2.0f, ImVec2(100, 8), "Y");

		ImGui::Text("Right Stick (%.2f, %.2f)", right.x, right.y);
		ImGui::ProgressBar((right.x + 1.0f) / 2.0f, ImVec2(100, 8), "X");
		ImGui::ProgressBar((right.y + 1.0f) / 2.0f, ImVec2(100, 8), "Y");

		// --- ���X�e�B�b�N�̉��� ---
		ImGui::Spacing();
		ImGui::Text("Left Stick Visual:");
		ImVec2 cursor = ImGui::GetCursorScreenPos();
		float radius = 48.0f;

		ImDrawList* dl = ImGui::GetWindowDrawList();
		ImVec2 center = ImVec2(cursor.x + radius, cursor.y + radius);
		dl->AddCircle(center, radius, IM_COL32(200, 200, 200, 100), 32, 2.0f);
		dl->AddCircle(center, radius * pad.deadZoneLeft, IM_COL32(100, 100, 100, 60), 32, 1.0f);

		ImVec2 stickPosL = ImVec2(center.x + left.x * radius, center.y + left.y * radius);
		dl->AddCircleFilled(stickPosL, 5.0f, IM_COL32(255, 100, 100, 255));

		ImGui::Dummy(ImVec2(radius * 2.0f, radius * 2.0f));

		// --- �E�X�e�B�b�N�̉��� ---
		ImGui::Spacing();
		ImGui::Text("Right Stick Visual:");
		cursor = ImGui::GetCursorScreenPos();
		center = ImVec2(cursor.x + radius, cursor.y + radius);
		dl->AddCircle(center, radius, IM_COL32(200, 200, 200, 100), 32, 2.0f);
		dl->AddCircle(center, radius * pad.deadZoneRight, IM_COL32(100, 100, 100, 60), 32, 1.0f);

		ImVec2 stickPosR = ImVec2(center.x + right.x * radius, center.y + right.y * radius);
		dl->AddCircleFilled(stickPosR, 5.0f, IM_COL32(100, 255, 100, 255));

		ImGui::Dummy(ImVec2(radius * 2.0f, radius * 2.0f));

		// --- D-Pad --- 
		sf::Vector2f dpad = pad.dpad;

		ImGui::Spacing();
		ImGui::Text("D-Pad (%.2f, %.2f)", dpad.x, dpad.y);

		// �����i���X�e�B�b�N�Ɠ����~�`��j
		ImVec2 cursorDpad = ImGui::GetCursorScreenPos();
		float dpadRadius = 32.0f; // �X�e�B�b�N��菬���߂�
		ImVec2 centerDpad = ImVec2(cursorDpad.x + dpadRadius, cursorDpad.y + dpadRadius);

		dl->AddCircle(centerDpad, dpadRadius, IM_COL32(200, 200, 200, 100), 32, 2.0f);
		dl->AddCircleFilled(ImVec2(centerDpad.x + dpad.x * dpadRadius, centerDpad.y + dpad.y * dpadRadius),
			5.0f, IM_COL32(100, 100, 255, 255));

		ImGui::Dummy(ImVec2(dpadRadius * 2.0f, dpadRadius * 2.0f));

		ImGui::Text("Left Trigger : %.2f", pad.triggerLeft);
		ImGui::ProgressBar(pad.triggerLeft, ImVec2(150, 8));
		ImGui::Text("Right Trigger: %.2f", pad.triggerRight);
		ImGui::ProgressBar(pad.triggerRight, ImVec2(150, 8));
	}

private:
	std::unordered_map<sf::Keyboard::Key, KeyState> m_keyStates;	//	�L�[�̓��͏��
	// SFML �̍ő�p�b�h���ɍ��킹���z��
	std::array<PadState, sf::Joystick::Count> m_pads;
	MouseState m_mouse;	//	�}�E�X�̏��\����
};

