#include "rsd/wrappers/buttonWrapper.hpp"
#include <fmt/core.h>

namespace rsd {
	ButtonWrapper::ButtonWrapper(const StreamDeckMessage& msg) : context(msg.context), id(msg.payload.settings.id), coords(msg.payload.coordinates) {
	}

	std::string ButtonWrapper::getTitle() { return title; }
	void ButtonWrapper::setTitle(const std::string& str) {
		title = str;
	}

	std::string ButtonWrapper::getId() { return id; }
	std::string ButtonWrapper::getContext() { return context; }

	bool ButtonWrapper::operator==(ButtonWrapper& rhs) {
		return context == rhs.context;
	}

	bool ButtonWrapper::operator!=(ButtonWrapper& rhs) {
		return context != rhs.context;
	}

	std::string ButtonWrapper::toString() {
		return fmt::format("Button({}, [c {}, r {}])", context, coords.row, coords.column);
	}

	void ButtonWrapper::registerLua(sol::state& lua) {
		lua.new_usertype<ButtonWrapper>("Button",
		    sol::no_constructor,
		    "getId", &ButtonWrapper::getId,
		    "getContext", &ButtonWrapper::getContext,
		    "getTitle", &ButtonWrapper::getTitle,
		    "setTitle", &ButtonWrapper::setTitle,

		    sol::meta_function::to_string, &ButtonWrapper::toString,
		    sol::meta_function::equal_to, &ButtonWrapper::operator==);
	}
}
