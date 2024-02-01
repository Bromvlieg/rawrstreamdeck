#pragma once

#include <memory>
#include <mutex>
#include <hv/WebSocketServer.h>
#include <hv/WebSocketClient.h>
#include <rsd/networking/client.hpp>

namespace rsd {
	class WSS {
		std::mutex lock;
		std::unique_ptr<hv::WebSocketServer> server;
		std::unique_ptr<hv::WebSocketService> service;
		std::vector<std::shared_ptr<Client>> clients;

		public:
			bool init();
			void shutdown();

			std::vector<std::shared_ptr<Client>> getClients();
	};
}
