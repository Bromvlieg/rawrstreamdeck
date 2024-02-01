#pragma once

#include <sol/sol.hpp>
#include <string>
#include "rsd/streamdeck/message.hpp"
#include "rsd/streamdeck/streamdeck.hpp"

namespace rsd {
	class DesktopWrapper {

	public:
		void sendKey(const std::string& key, bool isDown);

		std::string toString();
		static void registerLua(sol::state& lua);
	};
}
