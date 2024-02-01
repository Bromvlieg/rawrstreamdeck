#include "fmt/core.h"
#include <rsd/service.hpp>

int main(int argc, char *argv[])
{
	fmt::print("Meep :D\n");

	auto& service = rsd::Service::getInstance();

#ifndef _DEBUG
	try
	{
#endif
		if (!service.init()) return -1;
		service.run();
#ifndef _DEBUG
	}
	catch (const std::exception &err)
	{
		fmt::print("ERROR: {}\n", err.what());
		return -1;
	}
	catch (const std::string &err)
	{
		fmt::print("ERROR: {}\n", err);
		return -1;
	}
	catch (const char *err)
	{
		fmt::print("ERROR: {}\n", err);
		return -1;
	}
	catch (...)
	{
		fmt::print("ERROR: something died, cannot print what :(\n");
		return -1;
	}
#endif

	service.shutdown();
	return 0;
}
