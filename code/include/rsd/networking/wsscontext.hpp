#pragma once

#include <functional>
#include <rsd/networking/client.hpp>
#include <hv/WebSocketServer.h>

namespace rsd {
	class WSSContext {
		public:
		Client* client = nullptr;
		WebSocketChannelPtr channel;
		std::function<void(const StreamDeckMessage&)> onMessage;

		void send(const std::string& action, const nlohmann::json& data);
		void handleMessage(const std::string& msg, ws_opcode code);
	};
}
