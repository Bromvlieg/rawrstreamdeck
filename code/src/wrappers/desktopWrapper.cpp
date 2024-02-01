#include "rsd/wrappers/desktopWrapper.hpp"
#include <fmt/core.h>
#include "rsd/streamdeck/streamdeck.hpp"

#include <windows.h>

namespace rsd {
	void DesktopWrapper::sendKey(const std::string& key, bool isDown) {
		std::array<INPUT, 1> inputs;
		ZeroMemory(inputs.data(), sizeof(INPUT) * inputs.size());

		inputs[0].type = INPUT_KEYBOARD;
		inputs[0].ki.wVk = key.front();

		if (!isDown) {
			inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
		}

		UINT uSent = SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
		if (uSent != inputs.size()) {
			printf("SendInput failed: 0x%lx\n", HRESULT_FROM_WIN32(GetLastError()));
		}
	}

	std::string DesktopWrapper::toString() {
		return fmt::format("Desktop()");
	}

	void DesktopWrapper::registerLua(sol::state& lua) {
		lua.new_usertype<DesktopWrapper>("Desktop",
		    sol::no_constructor,
		    "sendKey", &DesktopWrapper::sendKey,

		    sol::meta_function::to_string, &DesktopWrapper::toString);
	}
}
