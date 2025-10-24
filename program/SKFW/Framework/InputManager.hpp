#pragma once
#include<unordered_map>

#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<SKFW/Framework/Singleton.hpp>
#include<imgui.h>

//	キーの状態
enum KeyState {
	none,
	pressed,
	held,
	released,
};

// Xboxボタン
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

// PSボタン
enum class PSButton {
	Cross = 0,   // Aに相当
	Circle = 1,  // Bに相当
	Square = 2,  // Xに相当
	Triangle = 3,// Yに相当
	L1 = 4,
	R1 = 5,
	Share = 6,
	Options = 7,
	L3 = 8,
	R3 = 9
};

// Switch Proボタン
enum class SwitchButton {
	B = 0,    // Aに相当
	A = 1,    // Bに相当
	Y = 2,    // Xに相当
	X = 3,    // Yに相当
	L = 4,
	R = 5,
	Minus = 6,
	Plus = 7,
	LStick = 8,
	RStick = 9
};


/// <summary>
/// 入力管理クラス、sfmlから入力状態を持ってきてフレームごとに行う。
/// </summary>
class InputManager : public Singleton<InputManager> {
	//	マウスの入力用
	struct MouseState {
		sf::Vector2i position;
		KeyState left = KeyState::none;
		KeyState right = KeyState::none;
	};

	//	PADの入力用
	struct PadState
	{
		std::unordered_map<unsigned int, KeyState> buttons;	//	ボタンごとの状態
		sf::Vector2f axisLeft{0.0f,0.0f};	//	左スティック
		sf::Vector2f axisRight{ 0.0f,0.0f };	//	右スティック
		sf::Vector2f dpad{ 0.0f, 0.0f };       // 十字キーの軸
		float triggerLeft = 0.0f;	//	Xbox用の左トリガー取得
		float triggerRight = 0.0f;	//	Xbox用の右トリガー取得
		float deadZoneLeft = 0.2f;
		float deadZoneRight = 0.2f;
		bool connected = false;	//	接続判定
	};

	//	キーの入力状態取得
	KeyState GetKeyState(sf::Keyboard::Key key)const {
		auto it = m_keyStates.find(key);
		if (it != m_keyStates.end()) return it->second;
		return KeyState::none;
	}

	//	Padの入力状態の更新
	void UpdatePads()
	{
		//	刺さっているPADの数分
		for (unsigned int id = 0; id < sf::Joystick::Count; id++)
		{
			//	接続判定
			PadState& pad = m_pads[id];
			pad.connected = sf::Joystick::isConnected(id);
			if (pad.connected == false)
			{
				continue;
			}

			//	ボタンの更新
			for (unsigned int button = 0; button < sf::Joystick::getButtonCount(id); button++)
			{
				//	押されているか判定
				bool pressed = sf::Joystick::isButtonPressed(id, button);
				auto& state = pad.buttons[button];
				//	押されていた場合（長押し判定用）
				if (pressed == true)
				{
					//	苗フレーム押されていたかどうか
					if (state != KeyState::held)
					{
						state = KeyState::pressed;
					}
				}
				//	押されていない時の前フレームの状態で離した瞬間かどうか
				else
				{
					//	前フレーム押されていたら
					if (state == KeyState::held || state == KeyState::pressed)
					{
						state = KeyState::released;
					}
					//	ないときは何もなし
					else
					{
						state = KeyState::none;
					}
				}
			}

			// 軸の傾き取得して -1.0f~1.0fにクランプ
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

				// トリガー値の更新（同時押し対応）
				float z = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::Z);
				float lt = 0.f, rt = 0.f;

				// Z軸は -100 ～ +100 の範囲を共有している
				if (z < 0)
					lt = -z / 100.f; // LTを押すと負方向
				else if (z > 0)
					rt = z / 100.f;  // RTを押すと正方向

				pad.triggerLeft = std::clamp(lt, 0.f, 1.f);
				pad.triggerRight = std::clamp(rt, 0.f, 1.f);
			}
			else // 不明など
			{
				pad.axisRight.x = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::U) / 100.f;
				pad.axisRight.y = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::V) / 100.f;
			}

			//pad.axisRight.x = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::Z) / 100.f;
			//pad.axisRight.y = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::R) / 100.f;


			// クランプしておくと安全
			pad.axisLeft.x = std::clamp(pad.axisLeft.x, -1.f, 1.f);
			pad.axisLeft.y = std::clamp(pad.axisLeft.y, -1.f, 1.f);
			pad.axisRight.x = std::clamp(pad.axisRight.x, -1.f, 1.f);
			pad.axisRight.y = std::clamp(pad.axisRight.y, -1.f, 1.f);

			pad.axisLeft = ApplyDeadZone(pad.axisLeft, pad.deadZoneLeft);
			pad.axisRight = ApplyDeadZone(pad.axisRight, pad.deadZoneRight);


			// --- D-Pad / 十字キー ---
			float dpadX = sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::PovX) / 100.f;
			float dpadY = -sf::Joystick::getAxisPosition(id, sf::Joystick::Axis::PovY) / 100.f; // 上が負なので反転

			// デッドゾーン補正（軽く）
			if (std::abs(dpadX) < 0.1f) dpadX = 0.f;
			if (std::abs(dpadY) < 0.1f) dpadY = 0.f;

			pad.dpad = { dpadX, dpadY };
		}
	}

	//	デッドゾーンを適応する
	sf::Vector2f ApplyDeadZone(const sf::Vector2f& input, float deadZone = 0.2f) {
		sf::Vector2f result = input;

		// 入力の大きさ
		float len = std::sqrt(input.x * input.x + input.y * input.y);

		if (len < deadZone) {
			result = { 0.f, 0.f };  // デッドゾーン内ならゼロ
		}
		else {
			// デッドゾーン外は0?1に再正規化
			float factor = (len - deadZone) / (1.f - deadZone);
			result = input / len * factor;
		}

		return result;
	}

	// マップ用に名前を出す
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
		// SFML は -100 .. 100 を返すので -1..1 に
		return raw100 / 100.0f;
	}


public:

	//	フレーム末で
	void Update() {
		//	キー状態を長押しか離している状態に変更
		for (auto& [key, state] : m_keyStates) {
			if (state == KeyState::pressed) state = KeyState::held;
			else if (state == KeyState::released) state = KeyState::none;
		}

		//	左
		if (m_mouse.left == KeyState::pressed) m_mouse.left = KeyState::held;
		else if (m_mouse.left == KeyState::released) m_mouse.left = KeyState::none;

		//	右
		if (m_mouse.right == KeyState::pressed) m_mouse.right = KeyState::held;
		if (m_mouse.right == KeyState::released) m_mouse.right = KeyState::none;

		// padの入力更新
		UpdatePads();
	}

	//	入力状態の更新
	void ProcessEvent(const sf::Event& event) {
		//	==========キーボード==========
		//	押し
		if (event.is<sf::Event::KeyPressed>()) {
			const auto* key = event.getIf<sf::Event::KeyPressed>();
			if (key) {
				auto& state = m_keyStates[key->code];
				if (state != KeyState::held) {
					state = KeyState::pressed;
				}
			}
		}
		//	離し
		else if (event.is<sf::Event::KeyReleased>()) {
			const auto* key = event.getIf<sf::Event::KeyReleased>();
			if (key) m_keyStates[key->code] = KeyState::released;
		}

		//	==========マウス==========
		//	押し
		else if (event.is<sf::Event::MouseButtonPressed>()) {
			const auto* m = event.getIf<sf::Event::MouseButtonPressed>();
			if (m) {
				if (m->button == sf::Mouse::Button::Left) m_mouse.left = KeyState::pressed;
				else if (m->button == sf::Mouse::Button::Right) m_mouse.right = KeyState::pressed;
			}
		}
		//	離し
		else if (event.is<sf::Event::MouseButtonReleased>()) {
			const auto* m = event.getIf<sf::Event::MouseButtonReleased>();
			if (m) {
				if (m->button == sf::Mouse::Button::Left) m_mouse.left = KeyState::released;
				else if (m->button == sf::Mouse::Button::Right)m_mouse.right = KeyState::released;
			}
		}
		//	マウス移動
		else if (event.is<sf::Event::MouseMoved>()) {
			const auto* m = event.getIf<sf::Event::MouseMoved>();
			if (m) m_mouse.position = m->position;
		}

	}

	//	-----キー判定----- //
	//	押した瞬間だけtrue
	bool IsKeyPressed(sf::Keyboard::Key key)const {
		return GetKeyState(key) == KeyState::pressed;
	}

	//	押している間true
	bool IsKeyHeld(sf::Keyboard::Key key)const {
		return GetKeyState(key) == KeyState::held;
	}

	//	離した瞬間
	bool IsKeyReleased(sf::Keyboard::Key key)const {
		return GetKeyState(key) == KeyState::released;
	}

	//	どれかのキーが押されていたらtrue
	bool IsAnyKeyDown()const {
		for (auto& [_, state] : m_keyStates) {
			if (state == KeyState::pressed || state == KeyState::held) return true;
		}
		return false;
	}

	//	-----マウス----- //
	//	押した瞬間だけtrue
	bool IsMousePressed(sf::Mouse::Button btn)const {
		if (btn == sf::Mouse::Button::Left) return m_mouse.left == KeyState::pressed;
		if (btn == sf::Mouse::Button::Right) return m_mouse.right == KeyState::pressed;
		return false;
	}

	//	押している間true
	bool IsMouseHeld(sf::Mouse::Button btn) const {
		if (btn == sf::Mouse::Button::Left) return m_mouse.left == KeyState::held;
		if (btn == sf::Mouse::Button::Right) return m_mouse.right == KeyState::held;
		return false;
	}

	//	離した瞬間true
	bool IsMouseReleased(sf::Mouse::Button btn) const {
		if (btn == sf::Mouse::Button::Left) return m_mouse.left == KeyState::released;
		if (btn == sf::Mouse::Button::Right) return m_mouse.right == KeyState::released;
		return false;
	}

	//	座標
	const sf::Vector2i& GetMousePosition() const { return m_mouse.position; }

	// ----- パッド判定 -----
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

	//	十字キーの軸の取得
	sf::Vector2f GetPadDPad(unsigned int id) const {
		if (id >= m_pads.size()) return { 0.f, 0.f };
		return m_pads[id].dpad;
	}

// トリガーが押されているか（一定以上）
bool IsPadTriggerLeft(unsigned int id) const {
    if (id >= m_pads.size()) return false;
    return m_pads[id].triggerLeft > 0.2f; // 20%以上押されたらtrue
}

bool IsPadTriggerRight(unsigned int id) const {
    if (id >= m_pads.size()) return false;
    return m_pads[id].triggerRight > 0.2f;
}


	//	デバック表示
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

		// --- 左スティックの可視化 ---
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

		// --- 右スティックの可視化 ---
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

		// 可視化（左スティックと同じ円描画）
		ImVec2 cursorDpad = ImGui::GetCursorScreenPos();
		float dpadRadius = 32.0f; // スティックより小さめに
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
	std::unordered_map<sf::Keyboard::Key, KeyState> m_keyStates;	//	キーの入力状態
	// SFML の最大パッド数に合わせた配列
	std::array<PadState, sf::Joystick::Count> m_pads;
	MouseState m_mouse;	//	マウスの情報構造体
};

