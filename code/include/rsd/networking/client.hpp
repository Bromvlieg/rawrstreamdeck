#pragma once

#include <nlohmann/json.hpp>
#include <map>
#include <memory>
#include <string>
#include "hv/json.hpp"
#include "rsd/script.hpp"
#include <hv/WebSocketServer.h>

#include <rsd/streamdeck/message.hpp>
#include <rsd/streamdeck/streamdeck.hpp>

namespace rsd {
	class WSSContext;

	class Client {
		WSSContext* context = nullptr;
		size_t id = 0;
		std::map<std::string, std::unique_ptr<Script>> scripts;

		std::map<std::string, std::function<void(Client&, const nlohmann::json&)>> callbacks;

		void net_init();

		public:
			WebSocketChannelPtr socket;
			StreamDeck deck;

			explicit Client(WSSContext* context);

			size_t getId();

			void registerMessages();
			void handleMessage(const StreamDeckMessage& msg);
	};
}
