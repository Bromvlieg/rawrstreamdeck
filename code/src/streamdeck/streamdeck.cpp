#include <magic_enum.hpp>
#include <rsd/streamdeck/streamdeck.hpp>
#include <rsd/networking/client.hpp>
#include <fmt/printf.h>
#include <fstream>
#include <sstream>
#include "base64/base64.hpp"

namespace rsd {
		std::string read_all(const std::filesystem::path &file_path) {
			std::ifstream ifs(file_path, std::ios::in | std::ios::binary | std::ios::ate);
			auto pos = ifs.tellg();
			if (pos == -1) {
				throw std::runtime_error("read_all: failed to open");
			}

			std::string result;
			if (pos == 0) {
				return result;
			}

			ifs.seekg(0, std::ios::beg);
			result.resize_and_overwrite(pos, [&](char *ptr, int n) {
				ifs.read(ptr, pos);
				return pos;
			});

			if (!ifs.good()) {
				throw std::runtime_error("read_all: failed to read to as expected");
			}

			return result;
		}

		StreamDeck::StreamDeck(Client* client) : client(client) {
			//auto bbbb = read_all("content/thumpsdown.jpg");
			//auto cccc = base64::to_base64(bbbb);
			//auto aaa = fmt::format("{},{}", "data:image/jpg;base64", cccc);
//
			//printf("");
		}

		const std::vector<StreamDeckButton>& StreamDeck::getButtons(){
			return buttons;
		}

		StreamDeckButton* StreamDeck::getButton(size_t row, size_t column) {
			for (auto& btn : buttons) {
				if (btn.row != row) continue;
				if (btn.column != column) continue;

				return &btn;
			}

			return nullptr;
		}

		void StreamDeck::addButton(const std::string& context, size_t row, size_t column) {
			auto oldBtn = getButton(row, column);
			if (oldBtn != nullptr) {
				oldBtn->context = context;
				return;
			}

			StreamDeckButton btn;
			btn.context = context;
			btn.row = row;
			btn.column = column;

			buttons.push_back(btn);
		}

		void StreamDeck::showOk(const std::string& context) {
			client->socket->send(nlohmann::json({
				{"event", "showOk"},
				{"context", context}
			}).dump());
		}

		void StreamDeck::showAlert(const std::string& context) {
			client->socket->send(nlohmann::json({
				{"event", "showAlert"},
				{"context", context}
			}).dump());
		}

		void StreamDeck::setTitle(const std::string& context, const std::string& title) {
			client->socket->send(nlohmann::json({
				{"event", "setTitle"},
				{"context", context},
				{"payload", {
					{"title", title}
				}}
			}).dump());

			//client->socket->send(nlohmann::json({
			//	{"event", "setFeedback"},
			//	{"context", context},
			//	{"payload", {
			//		{"id", "$B1"},
			//		{"items", {
			//			{
			//				{"key", "title"},
			//				{"type", "text"},
			//				{"value", "test"},
			//				{"alignment", "left"},
			//				{"rect", {16, 10, 136, 24}},
			//				{"font", {
			//					{"size", 16},
			//					{"weight", 600}
			//				}},
			//			}, {
			//				{"key", "icon"},
			//				{"type", "pixmap"},
			//				{"rect", {16, 40, 48, 48}}
			//			}, {
			//				{"key", "value"},
			//				{"type", "text"},
			//				{"alignment", "right"},
			//				{"rect", {76, 40, 108, 32}},
			//				{"font", {
			//					{"size", 24},
			//					{"weight", 600}
			//				}},
			//			}, {
			//				{"key", "indicator"},
			//				{"type", "bar"},
			//				{"value", 0},
			//				{"subtype", 4},
			//				{"border_w", 0},
			//				{"rect", {76, 74, 108, 12}}
			//			}
			//		}},
			//	}}
			//}).dump());
		}

		void StreamDeck::setImage(const std::string& context, const std::string& image) {
			auto contents = read_all(image);
			auto base64 = base64::to_base64(contents);
			auto fullStr = fmt::format("{},{}", "data:image/jpg;base64", base64);

			client->socket->send(nlohmann::json({
				{"event", "setImage"},
				{"context", context},
				{"payload", {
					{"image", fullStr}
				}}
			}).dump());

			//client->socket->send(nlohmann::json({
			//	{"event", "openUrl"},
			//	{"payload", {
			//		{"url", "https://mew.quest"}
			//	}}
			//}).dump());
		}

		void StreamDeck::querySettings(const std::string& context) {
			client->socket->send(nlohmann::json({
				{"event", "getSettings"},
				{"context", context}
			}).dump());
		}
} // namespace rsd
