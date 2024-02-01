#include <rsd/networking/wsscontext.hpp>
#include <fmt/printf.h>
#include "magic_enum.hpp"
#include "rsd/networking/client.hpp"

namespace rsd {
	void WSSContext::send(const std::string& action, const nlohmann::json& data) {
		channel->send(nlohmann::json({{"action", action}, {"data", data}}).dump());
	}

	void WSSContext::handleMessage(const std::string& msg, ws_opcode code) {
		try {
			auto json = nlohmann::json::parse(msg);
			StreamDeckMessage m = json;
			if (m.event == StreamDeckEvent::none) {
				fmt::print("Error, invalid msg :(\n{}\n", msg);
				return;
			}

			fmt::print("Received: {}\n", magic_enum::enum_name(m.event));
			onMessage(m);
		} catch (const std::exception& err) {
			fmt::print("Error :(\n{}\n", err.what());
			send("error", err.what());
			return;
		}
	}
}
