#pragma once

#include <sol/sol.hpp>
#include <string>
#include "rsd/streamdeck/message.hpp"

namespace rsd {
	class ButtonWrapper {
		std::string context;
		std::string id;
		std::string title;
		StreamDeckCoordinates coords;

	public:
		explicit ButtonWrapper(const StreamDeckMessage& msg);

		std::string getId();
		std::string getTitle();
		std::string getContext();

		void setTitle(const std::string& str);

		std::string toString();
		bool operator==(ButtonWrapper& rhs);
		bool operator!=(ButtonWrapper& rhs);

		static void registerLua(sol::state& lua);
	};
}
