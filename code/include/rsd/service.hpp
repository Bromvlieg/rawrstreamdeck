#pragma once

#include <rsd/networking/wss.hpp>

namespace rsd {
	class Service {
		public:
			static Service& getInstance();

			WSS wss;

			bool init();
			void run();
			void shutdown();
	};
}
