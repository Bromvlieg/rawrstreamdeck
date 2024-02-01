#include <rsd/networking/wss.hpp>
#include <rsd/networking/wsscontext.hpp>
#include <fmt/printf.h>
#include <functional>

using namespace hv;

namespace rsd {
	bool WSS::init() {
		service = std::make_unique<WebSocketService>();
		service->onopen = [this](const WebSocketChannelPtr& channel, const HttpRequestPtr& req) {
			printf("onopen: GET %s\n", req->Path().c_str());
			auto ctx = channel->newContextPtr<WSSContext>();
			ctx->channel = channel;

			auto client = std::make_shared<Client>(ctx.get());
			ctx->client = client.get();

			client->registerMessages();
			fmt::print("Connect client {} connected\n", client->getId());

			std::lock_guard<std::mutex> lockg(lock);
			clients.push_back(std::move(client));
		};

		service->onmessage = [](const WebSocketChannelPtr& channel, const std::string& msg) {
			auto ctx = channel->getContextPtr<WSSContext>();
			ctx->handleMessage(msg, channel->opcode);
		};

		service->onclose = [this](const WebSocketChannelPtr& channel) {
			auto ctx = channel->getContextPtr<WSSContext>();
			fmt::print("Disconnect client {} connected\n", ctx->client->getId());

			std::lock_guard<std::mutex> lockg(lock);
			for (size_t i = 0; i < clients.size(); i++) {
				if (clients[i]->getId() != ctx->client->getId()) continue;
				clients.erase(clients.begin() + i);
				break;
			}
		};

		server = std::make_unique<WebSocketServer>();
		server->port = 1992;
		server->registerWebSocketService(service.get());
		if (server->start() != 0) {
			fmt::print("error :(\n");
			return false;
		}

		return true;
	}

	std::vector<std::shared_ptr<Client>> WSS::getClients() {
		std::lock_guard<std::mutex> lockg(lock);
		return clients;
	}

	void WSS::shutdown() {
		server.reset();
		service.reset();
	}
}
