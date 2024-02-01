#include "rsd/wrappers/deckWrapper.hpp"
#include <fmt/core.h>
#include "rsd/streamdeck/streamdeck.hpp"

namespace rsd {
	DeckWrapper::DeckWrapper(StreamDeck& deck) : deck(&deck) {
	}

	void DeckWrapper::setTitle(const std::string& context, const std::string& title) {
		deck->setTitle(context, title);
	}

	void DeckWrapper::showAlert(const std::string& context) {
		deck->showAlert(context);
	}

	void DeckWrapper::showOk(const std::string& context) {
		deck->showOk(context);
	}

	bool DeckWrapper::operator==(DeckWrapper& rhs) {
		return deck == rhs.deck;
	}

	bool DeckWrapper::operator!=(DeckWrapper& rhs) {
		return deck != rhs.deck;
	}

	std::string DeckWrapper::toString() {
		return fmt::format("Deck()");
	}

	void DeckWrapper::registerLua(sol::state& lua) {
		lua.new_usertype<DeckWrapper>("Deck",
		    sol::no_constructor,
		    "setTitle", &DeckWrapper::setTitle,
		    "showOk", &DeckWrapper::showOk,
		    "showAlert", &DeckWrapper::showAlert,

		    sol::meta_function::to_string, &DeckWrapper::toString,
		    sol::meta_function::equal_to, &DeckWrapper::operator==);
	}
}
