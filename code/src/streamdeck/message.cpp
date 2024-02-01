#include <magic_enum.hpp>
#include <rsd/streamdeck/message.hpp>

namespace rsd {
	void from_json(const nlohmann::json& j, StreamDeckMessage& o) {
		o.action = j.value("action", "unknown");
		o.context = j.value("context", "unknown");
		o.device = j.value("device", "unknown");

		o.event = magic_enum::enum_cast<StreamDeckEvent>(j.value("event", "none")).value_or(StreamDeckEvent::none);
		o.payload = j.value("payload", StreamDeckPayload());
	}

	void from_json(const nlohmann::json& j, StreamDeckPayload& o) {
		o.multiAction =  j.value("isInMultiAction", false);

		o.coordinates = j.value("coordinates", StreamDeckCoordinates());
		o.settings = j.value("settings", StreamDeckSettings());
	}

	void from_json(const nlohmann::json& j, StreamDeckSettings& o) {
		o.id = j.value("id", "unknown");
		o.remoteServer = j.value("remoteServer", "unknown");
	}

	void from_json(const nlohmann::json& j, StreamDeckCoordinates& o) {
		o.column = j.value("column", 0);
		o.row = j.value("row", 0);
	}
} // namespace rsd
