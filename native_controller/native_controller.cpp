#include "stdafx.h"
#include "native_controller.h"

#include <cmath>
#include <winerror.h>

using controller_button = bool(*)();
struct controller {
	const DWORD user_index;
	DWORD last_packet_number = -1;
	DWORD error = ERROR_SUCCESS;
	XINPUT_STATE state{};
	XINPUT_BATTERY_INFORMATION battery{};
	controller(const DWORD user_index) : user_index(user_index) {};
};

std::array<controller, 4> controllers{0,1,2,3};

bool native::controller::update() {
	bool controller_state_changed = false;
	for(auto& pad : controllers) {
		if(pad.error == ERROR_SUCCESS) {
			pad.error = XInputGetState(pad.user_index, &pad.state);

			if(pad.state.dwPacketNumber != pad.last_packet_number)//state has changed
				controller_state_changed = true;

			pad.last_packet_number = pad.state.dwPacketNumber;
		}
	}
	return controller_state_changed;
}

bool native::controller::reconnect_and_update() {
	char starting_connectivity = 0; //connectivity bitarray
	for(auto& pad : controllers) {
		if(pad.error == ERROR_SUCCESS)
			starting_connectivity += 1 << pad.user_index;
		pad.error = ERROR_SUCCESS;
	}
	update();
	char ending_connectivity = 0;
	for(auto& pad : controllers) {
		if(pad.error == ERROR_SUCCESS)
			ending_connectivity += 1 << pad.user_index;
	}
	return starting_connectivity != ending_connectivity;
}

unsigned char native::controller::get_UDLR(bool use_L, bool use_R, bool use_dpad) {
	char ret = 0;
	if((use_L&&is_L_up()) || (use_dpad&&is_dpad_up()) || (use_R&&is_R_up()))
		ret |= UP;
	if((use_L&&is_L_down()) || (use_dpad&&is_dpad_down()) || (use_R&&is_R_down()))
		ret |= DOWN;
	if((use_L&&is_L_left()) || (use_dpad&&is_dpad_left()) || (use_R&&is_R_left()))
		ret |= LEFT;
	if((use_L&&is_L_right()) || (use_dpad&&is_dpad_right()) || (use_R&&is_R_right()))
		ret |= RIGHT;
	return ret;
}


std::pair<short, short> native::controller::get_L_xy() {
	for(auto& pad : controllers) {
		if(abs(pad.state.Gamepad.sThumbLX) >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
			|| abs(pad.state.Gamepad.sThumbLY) >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return{pad.state.Gamepad.sThumbLX, pad.state.Gamepad.sThumbLY};
	}
	return{0,0};
}

std::pair<float, float> native::controller::get_L_xy_float() {
	for(auto& pad : controllers) {
		if(abs(pad.state.Gamepad.sThumbLX) >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
			|| abs(pad.state.Gamepad.sThumbLY) >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return{pad.state.Gamepad.sThumbLX < 0 ? pad.state.Gamepad.sThumbLX / 32768.0f : pad.state.Gamepad.sThumbLX / 32767.0f,
			pad.state.Gamepad.sThumbLY < 0 ? pad.state.Gamepad.sThumbLY / 32768.0f : pad.state.Gamepad.sThumbLY / 32767.0f};
	}
	return{0.0f, 0.0f};
}

std::pair<short, short> native::controller::get_R_xy() {
	for(auto& pad : controllers) {
		if(abs(pad.state.Gamepad.sThumbRX) >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
			|| abs(pad.state.Gamepad.sThumbRY) >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return {pad.state.Gamepad.sThumbRX, pad.state.Gamepad.sThumbRY};
	}
	return {0,0};
}

std::pair<float, float> native::controller::get_R_xy_float() {
	for(auto& pad : controllers) {
		if(abs(pad.state.Gamepad.sThumbRX) >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
			|| abs(pad.state.Gamepad.sThumbRY) >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return{pad.state.Gamepad.sThumbRX < 0 ? pad.state.Gamepad.sThumbRX / 32768.0f : pad.state.Gamepad.sThumbRX / 32767.0f,
			pad.state.Gamepad.sThumbRY < 0 ? pad.state.Gamepad.sThumbRY / 32768.0f : pad.state.Gamepad.sThumbRY / 32767.0f};
	}
	return {0.0f, 0.0f};
}

bool native::controller::is_LB_pressed() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER)
			return true;
	}
	return false;
}

bool native::controller::is_RB_pressed() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER)
			return true;
	}
	return false;
}

bool native::controller::is_L_pressed() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_THUMB)
			return true;
	}
	return false;
}

bool native::controller::is_R_pressed() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_THUMB)
			return true;
	}
	return false;
}

bool native::controller::is_start_pressed() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_START)
			return true;
	}
	return false;
}

bool native::controller::is_select_pressed() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_BACK)
			return true;
	}
	return false;
}

bool native::controller::is_A_pressed() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_A)
			return true;
	}
	return false;
}

bool native::controller::is_B_pressed() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_B)
			return true;
	}
	return false;
}

bool native::controller::is_X_pressed() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_X)
			return true;
	}
	return false;
}

bool native::controller::is_Y_pressed() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_Y)
			return true;
	}
	return false;
}

bool native::controller::is_L_up() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return true;
	}
	return false;
}

bool native::controller::is_L_down() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return true;
	}
	return false;
}

bool native::controller::is_L_left() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return true;
	}
	return false;
}

bool native::controller::is_L_right() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return true;
	}
	return false;
}

bool native::controller::is_R_up() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return true;
	}
	return false;
}

bool native::controller::is_R_down() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return true;
	}
	return false;
}

bool native::controller::is_R_left() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return true;
	}
	return false;
}

bool native::controller::is_R_right() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return true;
	}
	return false;
}

bool native::controller::is_dpad_up() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP)
			return true;
	}
	return false;
}

bool native::controller::is_dpad_down() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN)
			return true;
	}
	return false;
}

bool native::controller::is_dpad_left() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT)
			return true;
	}
	return false;
}

bool native::controller::is_dpad_right() {
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT)
			return true;
	}
	return false;
}

bool native::controller::is_LT_pressed() {
	return get_left_trigger() > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

bool native::controller::is_RT_pressed() {
	return get_right_trigger() > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

unsigned char native::controller::get_left_trigger() {//returning the most-held-down trigger feels more natural
	unsigned char lt = 0;
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.bLeftTrigger > lt)
			lt = pad.state.Gamepad.bLeftTrigger;
	}
	return lt;
}

float native::controller::get_left_trigger_float() {
	return get_left_trigger() / 255.0f;
}

unsigned char native::controller::get_right_trigger() {//returning the most-held-down trigger feels more natural
	unsigned char rt = 0;
	for(auto& pad : controllers) {
		if(pad.state.Gamepad.bRightTrigger > rt)
			rt = pad.state.Gamepad.bRightTrigger;
	}
	return rt;
}

float native::controller::get_right_trigger_float() {
	return get_right_trigger() / 255.0f;
}

void native::controller::set_vibration(unsigned short left_motor, unsigned short right_motor, const char user_index) {
	XINPUT_VIBRATION vibe{left_motor,right_motor};
	XInputSetState(user_index, &vibe);
}

void native::controller::clear_vibration() {
	for(int i = 0; i < 4; i++) {
		set_vibration(0, 0, i);
	}
}

std::vector<controller_button> native::controller::get_functions_of_pressed_buttons() {
	std::vector<controller_button> pressed_buttons{};
	for(auto& b : {
		is_LB_pressed, is_RB_pressed,
		is_L_pressed, is_R_pressed,
		is_start_pressed, is_select_pressed,
		is_A_pressed, is_B_pressed, is_X_pressed, is_Y_pressed,
		is_L_up, is_L_down, is_L_left, is_L_right,
		is_R_up, is_R_down, is_R_left, is_R_right,
		is_dpad_up, is_dpad_down, is_dpad_left, is_dpad_right,
		is_LT_pressed, is_RT_pressed
		}) {
		if(b())
			pressed_buttons.emplace_back(b);
	}

	return pressed_buttons;
}

static const std::unordered_map<controller_button, std::string> input_names{
	{native::controller::is_LB_pressed, "LB"},
{native::controller::is_RB_pressed, "RB"},
{native::controller::is_L_pressed, "L"},
{native::controller::is_R_pressed, "R"},
{native::controller::is_start_pressed, "Start"},
{native::controller::is_select_pressed, "Back"},
{native::controller::is_A_pressed, "A"},
{native::controller::is_B_pressed, "B"},
{native::controller::is_X_pressed, "X"},
{native::controller::is_Y_pressed, "Y"},

{native::controller::is_L_up, "L Up"},
{native::controller::is_L_down, "L Down"},
{native::controller::is_L_left, "L Left"},
{native::controller::is_L_right, "L Right"},

{native::controller::is_R_up, "R Up"},
{native::controller::is_R_down, "R Down"},
{native::controller::is_R_left, "R Left"},
{native::controller::is_R_right, "R Right"},

{native::controller::is_dpad_up, "DPad Up"},
{native::controller::is_dpad_down, "DPad Down"},
{native::controller::is_dpad_left, "DPad Left"},
{native::controller::is_dpad_right, "DPad Right"},

{native::controller::is_LT_pressed, "LT"},
{native::controller::is_RT_pressed, "RT"}
};

std::string native::controller::get_button_name(const controller_button button_function) {
	return input_names.at(button_function);
}

std::vector<std::string> native::controller::get_button_names(const std::vector<const controller_button>& button_functions) {
	std::vector<std::string> names{};
	for(const controller_button bf : button_functions) {
		names.emplace_back(get_button_name(bf));
	}
	return names;
}

const std::unordered_map<controller_button, std::string>& native::controller::get_buttons_and_names() {
	return input_names;
}

static const std::unordered_map<std::string, controller_button> name_to_button{
	{"LB",native::controller::is_LB_pressed},
{"RB",native::controller::is_RB_pressed},
{"L",native::controller::is_L_pressed},
{"R",native::controller::is_R_pressed},
{"Start",native::controller::is_start_pressed},
{"Back",native::controller::is_select_pressed},
{"A",native::controller::is_A_pressed},
{"B",native::controller::is_B_pressed},
{"X",native::controller::is_X_pressed},
{"Y",native::controller::is_Y_pressed},

{"L Up",native::controller::is_L_up},
{"L Down",native::controller::is_L_down},
{"L Left",native::controller::is_L_left},
{"L Right",native::controller::is_L_right},

{"R Up",native::controller::is_R_up},
{"R Down",native::controller::is_R_down},
{"R Left",native::controller::is_R_left},
{"R Right",native::controller::is_R_right},

{"DPad Up",native::controller::is_dpad_up},
{"DPad Down",native::controller::is_dpad_down},
{"DPad Left",native::controller::is_dpad_left},
{"DPad Right",native::controller::is_dpad_right},

{"LT",native::controller::is_LT_pressed},
{"RT",native::controller::is_RT_pressed},
};

const controller_button native::controller::get_button_from_name(const std::string& s) {
	auto c = name_to_button.find(s);
	if(c == name_to_button.end()) {
		return nullptr;
	} else {
		return c->second;
	}
}

void native::controller::clear_state() {
	for(auto& pad : controllers) {
		pad.state.Gamepad.wButtons = 0;
		pad.state.Gamepad.bLeftTrigger = pad.state.Gamepad.bLeftTrigger = 0;
		pad.state.Gamepad.sThumbLX =
			pad.state.Gamepad.sThumbLY =
			pad.state.Gamepad.sThumbRX =
			pad.state.Gamepad.sThumbRY = 0;
	}
}

bool native::controller::is_connected() {
	for(auto& pad : controllers) {
		if(pad.error == ERROR_SUCCESS)
			return true;
	}
	return false;
}

std::array<bool, 4> native::controller::get_connected_controllers() {
	return {
		controllers[0].error == ERROR_SUCCESS,
		controllers[1].error == ERROR_SUCCESS,
		controllers[2].error == ERROR_SUCCESS,
		controllers[3].error == ERROR_SUCCESS,
	};
}

std::ostream& native::controller::operator<<(std::ostream& os, const battery_status b) {
	switch(b) {
	case native::controller::battery_status::EMPTY:
		os << "empty";
		break;
	case native::controller::battery_status::LOW:
		os << "low";
		break;
	case native::controller::battery_status::MEDIUM:
		os << "medium";
		break;
	case native::controller::battery_status::FULL:
		os << "full";
		break;
	case native::controller::battery_status::WIRED:
		os << "wired";
		break;
	case native::controller::battery_status::DISCONNECTED:
		os << "disconnected";
		break;
	default:
		os << "unknown";
		break;
	}
	return os;
}

native::controller::battery_status native::controller::get_battery_level(const char user_index) {
	XInputGetBatteryInformation(user_index, BATTERY_DEVTYPE_GAMEPAD, &controllers[user_index].battery);
	switch(controllers[user_index].battery.BatteryType) {
	case BATTERY_TYPE_DISCONNECTED:
		return native::controller::battery_status::DISCONNECTED;
	case BATTERY_TYPE_NIMH://fallthrough
	case BATTERY_TYPE_ALKALINE: {
		switch(controllers[user_index].battery.BatteryLevel) {
		case BATTERY_LEVEL_EMPTY:
			return native::controller::battery_status::EMPTY;
		case BATTERY_LEVEL_LOW:
			return native::controller::battery_status::LOW;
		case BATTERY_LEVEL_MEDIUM:
			return native::controller::battery_status::MEDIUM;
		case BATTERY_LEVEL_FULL:
			return native::controller::battery_status::FULL;
		default:
			return native::controller::battery_status::UNKNOWN;
		}
	}
	case BATTERY_TYPE_WIRED:
		return native::controller::battery_status::WIRED;
	default:
		return native::controller::battery_status::UNKNOWN;
	}
}

std::array<native::controller::battery_status, 4> native::controller::get_battery_levels() {
	return {
		get_battery_level(0),
		get_battery_level(1),
		get_battery_level(2),
		get_battery_level(3)
	};
}

bool native::controller::is_controller_nonneutral(const char user_index) {
	const XINPUT_GAMEPAD& pad{controllers[user_index].state.Gamepad};
	if(
		pad.wButtons ||
		pad.bLeftTrigger  > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ||
		pad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ||
		abs(pad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		abs(pad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		abs(pad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		abs(pad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
		) {
		return true;
	} else {
		return false;
	}
}

std::array<bool, 4> native::controller::get_nonneutral_controllers() {
	return {
		is_controller_nonneutral(0),
		is_controller_nonneutral(1),
		is_controller_nonneutral(2),
		is_controller_nonneutral(3)
	};
}
