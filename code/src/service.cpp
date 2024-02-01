
#include <rsd/service.hpp>
#include <fmt/printf.h>
#include <exception>
#include <chrono>

namespace rsd {
	Service& Service::getInstance() {
		static Service obj;
		return obj;
	}

	bool Service::init()
	{
		wss.init();
		return true;
	}

	void Service::shutdown()
	{
		wss.shutdown();
	}

	void Service::run()
	{
		//log::print(LogLevel::info, "entering loop");
		while(true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
}
