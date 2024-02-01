#pragma once

#include <sol/sol.hpp>
#include <string>
#include "rsd/streamdeck/message.hpp"
#include "rsd/streamdeck/streamdeck.hpp"

namespace rsd {
	class DeckWrapper {
		StreamDeck* deck;

	public:
		explicit DeckWrapper(StreamDeck& deck);

		void setTitle(const std::string& context, const std::string& str);
		void showOk(const std::string& context);
		void showAlert(const std::string& context);

		std::string toString();
		bool operator==(DeckWrapper& rhs);
		bool operator!=(DeckWrapper& rhs);

		static void registerLua(sol::state& lua);
	};
}
