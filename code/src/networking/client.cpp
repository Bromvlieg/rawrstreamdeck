#include <magic_enum.hpp>
#include <rsd/networking/client.hpp>
#include <rsd/networking/wsscontext.hpp>

#include <fmt/printf.h>
#include <stdio.h>
#include <winuser.h>

#include <exception>
#include <fstream>
#include <memory>
#include <sstream>
#include <array>

#include "base64/base64.hpp"
#include "rsd/service.hpp"
#include "rsd/streamdeck/message.hpp"

namespace rsd {
	Client::Client(WSSContext* context)
	    : socket(context->channel), context(context), deck(this) {
		static size_t GID = 0;
		id = ++GID;
	}

	size_t Client::getId() { return id; }

	void Client::registerMessages() {
		context->onMessage = [this](const StreamDeckMessage& msg) {
			handleMessage(msg);
		};
	}

	std::vector<std::string> strSplit(const std::string& str, char splitter) {
		std::stringstream test(str);
		std::string segment;
		std::vector<std::string> seglist;

		while(std::getline(test, segment, splitter)) {
			seglist.push_back(segment);
		}

		return seglist;
	}

	void Client::handleMessage(const StreamDeckMessage& msg) {
		if (msg.payload.settings.id.empty()) return;

		auto parts = strSplit(msg.payload.settings.id, ' ');
		auto scriptName = parts[0];
		parts.erase(parts.begin());

		if (scripts.find(scriptName) == scripts.end()) {
			scripts[scriptName] = std::make_unique<Script>("./lua/" + scriptName);
			scripts[scriptName]->init();
		}

		auto& script = scripts[scriptName];
		script->setDeck(deck);

		if (msg.event == StreamDeckEvent::willAppear) {
			deck.addButton(msg.context, msg.payload.coordinates.row, msg.payload.coordinates.column);

			script->onWillAppear(msg);
			return;
		}

		// fallback incase we missed the message
		if (deck.getButton(msg.payload.coordinates.row, msg.payload.coordinates.column) == nullptr) {
			deck.addButton(msg.context, msg.payload.coordinates.row, msg.payload.coordinates.column);

			script->onWillAppear(msg);
		}

		if (msg.event == StreamDeckEvent::willDisappear) {
			script->onWillDisappear(msg);
		}

		if (msg.event == StreamDeckEvent::keyDown) {
			fmt::print("Pressed [{}, {}]\n", msg.payload.coordinates.row, msg.payload.coordinates.column);
			script->onPressDown(msg);
		}

		if (msg.event == StreamDeckEvent::keyUp) {
			fmt::print("Released [{}, {}]\n", msg.payload.coordinates.row, msg.payload.coordinates.column);
			script->onPressUp(msg);
			//sendKey('a');
		}
	}
} // namespace rsd
