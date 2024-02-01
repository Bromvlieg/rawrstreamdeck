#pragma once

#include <sol/sol.hpp>
#include <string>
#include <filesystem>
#include "rsd/streamdeck/message.hpp"
#include "rsd/streamdeck/streamdeck.hpp"

namespace rsd {
	class Script {
		sol::state lua;
		std::string folder;
		std::string name;
		std::string errorString;
		std::string entrypoint = "main.lua";

		void callFunc(const StreamDeckMessage& msg, const std::string& name);

	public:
		explicit Script(const std::filesystem::path& path);
		void createLua();

		void lua_include(const std::string& path, sol::this_state state);
		void lua_print(const std::string& str, sol::variadic_args va);

		void init();
		void shutdown();

		void setDeck(StreamDeck& deck);

		void onWillAppear(const StreamDeckMessage& msg);
		void onWillDisappear(const StreamDeckMessage& msg);
		void onPressDown(const StreamDeckMessage& msg);
		void onPressUp(const StreamDeckMessage& msg);
	};
}
