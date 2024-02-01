#pragma once

#include <nlohmann/json.hpp>
#include <vector>

namespace rsd {
	class Client;

	struct StreamDeckButton {
		size_t column = 0;
		size_t row = 0;
		std::string context;
	};

	class StreamDeck {
		Client* client = nullptr;
		std::vector<StreamDeckButton> buttons;

	public:
		explicit StreamDeck(Client* client);

		const std::vector<StreamDeckButton>& getButtons();
		StreamDeckButton* getButton(size_t row, size_t column);
		void addButton(const std::string& context, size_t row, size_t column);

		void setTitle(const std::string& context, const std::string& title);
		void setImage(const std::string& context, const std::string& image);

		void showOk(const std::string& context);
		void showAlert(const std::string& context);

		void querySettings(const std::string& context);
	};
}
