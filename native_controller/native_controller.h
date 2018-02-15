#pragma once

#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

#include <utility>
#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <iostream>
#include <cstdint>

namespace native {
	namespace controller {//OS-specific containment namespace

		bool update();//returns if controller state has changed, does not poll disconnected controllers
		bool reconnect_and_update();//rechecks controller connectivity, returns true if connectivity has changed, and updates controller state in the process

		unsigned char get_UDLR(bool use_L, bool use_R, bool use_dpad);

		std::pair<short, short> get_L_xy();
		std::pair<float, float> get_L_xy_float();

		std::pair<short, short> get_R_xy();
		std::pair<float, float> get_R_xy_float();

		bool is_LB_pressed(), is_RB_pressed();
		bool is_L_pressed(), is_R_pressed();
		bool is_start_pressed(), is_select_pressed();
		bool is_A_pressed(), is_B_pressed(), is_X_pressed(), is_Y_pressed();
		bool is_L_up(), is_L_down(), is_L_left(), is_L_right();
		bool is_R_up(), is_R_down(), is_R_left(), is_R_right();
		bool is_dpad_up(), is_dpad_down(), is_dpad_left(), is_dpad_right();
		bool is_LT_pressed(), is_RT_pressed();

		unsigned char get_left_trigger();
		float get_left_trigger_float();
		unsigned char get_right_trigger();
		float get_right_trigger_float();

		void set_vibration(const unsigned short left_motor, const unsigned short right_motor, const char user_index);//left motor is heavier
		void clear_vibration();

		using controller_button = bool(*)();
		std::vector<controller_button> get_functions_of_pressed_buttons();

		std::string get_button_name(const controller_button);
		std::vector<std::string> get_button_names(const std::vector<const controller_button>& buttons);

		const std::unordered_map<controller_button, std::string>& get_buttons_and_names();

		const controller_button get_button_from_name(const std::string& name);//if string does not match, returns nullptr

		void clear_state();

		bool is_connected();
		std::array<bool, 4> get_connected_controllers();

		enum class battery_status {
			EMPTY, LOW, MEDIUM, FULL, WIRED, UNKNOWN, DISCONNECTED
		};
		std::ostream& operator<<(std::ostream& os, const battery_status b);

		battery_status get_battery_level(const char user_index);
		std::array<battery_status, 4> get_battery_levels();

		bool is_controller_nonneutral(const char user_index);
		std::array<bool, 4> get_nonneutral_controllers();

		enum UDLR {
			UP = 0x80,
			DOWN = 0x40,
			LEFT = 0x20,
			RIGHT = 0x10
		};
	};
}