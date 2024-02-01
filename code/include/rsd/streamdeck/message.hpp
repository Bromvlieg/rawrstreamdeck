#pragma once

#include <nlohmann/json.hpp>

namespace rsd {
	enum class StreamDeckEvent {
		none,
		keyUp,
		keyDown,
		willAppear,
		willDisappear,
		didReceiveSettings,
	};

	struct StreamDeckCoordinates {
		size_t column = 0;
		size_t row = 0;
	};

	struct StreamDeckSettings {
		std::string id;
		std::string remoteServer;
	};

	struct StreamDeckPayload {
		bool multiAction = false;

		StreamDeckCoordinates coordinates;
		StreamDeckSettings settings;
	};

	struct StreamDeckMessage {
		std::string action;
		std::string context;
		std::string device;
		StreamDeckEvent event;
		StreamDeckPayload payload;
	};

	void from_json(const nlohmann::json& j, StreamDeckMessage& o);
	void from_json(const nlohmann::json& j, StreamDeckPayload& o);
	void from_json(const nlohmann::json& j, StreamDeckSettings& o);
	void from_json(const nlohmann::json& j, StreamDeckCoordinates& o);
}
