
#include <rsd/script.hpp>
#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/args.h>
#include <filesystem>
#include "rsd/wrappers/buttonWrapper.hpp"
#include "rsd/wrappers/deckWrapper.hpp"
#include "rsd/wrappers/desktopWrapper.hpp"
#include "sol/forward.hpp"
#include <magic_enum.hpp>

namespace rsd {
	template <typename T>
	void registerEnum(sol::state& lua, const std::string& name) {
		auto names = magic_enum::enum_names<T>();
		auto vals = magic_enum::enum_values<T>();

		auto tbl = lua.create_table();
		for (size_t i = 0; i < names.size(); i++) {
			tbl[names[i]] = vals[i];
			tbl[vals[i]] = names[i];
		}

		lua[name] = tbl;
	}

	void replaceAll(std::string& str, const std::string& from, const std::string& to) {
		if (from.empty()) return;

		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
	}

	Script::Script(const std::filesystem::path& path) {
		folder = path.string() + "/";
		name = path.filename().string();
	}

	void Script::init() {
		createLua();
	}

	void Script::shutdown() {
	}

	void Script::createLua() {
		lua = {};

		lua.open_libraries(sol::lib::base);
		lua.open_libraries(sol::lib::package);
		lua.open_libraries(sol::lib::math);
		lua.open_libraries(sol::lib::table);
		lua.open_libraries(sol::lib::string);
		lua.open_libraries(sol::lib::coroutine);
		lua.open_libraries(sol::lib::bit32);
		lua.open_libraries(sol::lib::jit);
		lua.open_libraries(sol::lib::debug);
		lua.open_libraries(sol::lib::os);

		ButtonWrapper::registerLua(lua);
		DeckWrapper::registerLua(lua);
		DesktopWrapper::registerLua(lua);

		lua["print"] = [this](const std::string& str, sol::variadic_args va) { lua_print(str, va); };
		lua["include"] = [this](const std::string& path, sol::this_state state) { lua_include(path, state); };

		lua["script"] = lua.create_table();
		lua["desktop"] = DesktopWrapper();

		try {
			//fileTimes[fullpath + entrypoint] = std::filesystem::last_write_time(fullpath + entrypoint);
			sol::protected_function_result result = lua.safe_script_file(folder + entrypoint);
			if (result.valid()) return;

			sol::error err = result;
			errorString = err.what();
			replaceAll(errorString, folder, "");

			fmt::print("[LUA ERROR] {}\n", errorString);
			//owner->addLog(errorString);
		} catch (const sol::error& err) {
			errorString = err.what();
			replaceAll(errorString, folder, "");

			fmt::print("[LUA ERROR] {}\n", errorString);
			//owner->addLog(errorString);
		} catch (const std::exception& err) {
			errorString = err.what();
			replaceAll(errorString, folder, "");

			fmt::print("[LUA ERROR] {}\n", errorString);
			//owner->addLog(errorString);
		} catch (...) {
			fmt::print("[LUA ERROR] unknown error :(\n");
			//owner->addLog("");
		}
	}

	void Script::lua_include(const std::string& path, sol::this_state state) {
		auto fixedPath = folder + path;

		if (!std::filesystem::exists(fixedPath)) {
			luaL_error(state.L, "%s", fmt::format("File does not exist: {}", path).c_str());
			return;
		}

		//fileTimes[fixedPath] = std::filesystem::last_write_time(fixedPath);
		sol::protected_function_result result = lua.safe_script_file(fixedPath);
		if (result.valid()) return;

		sol::error err = result;
		luaL_error(state.L, "%s", err.what());
	}

	void Script::lua_print(const std::string& str, sol::variadic_args va) {
		auto vars = std::vector<sol::object>(va.begin(), va.end());
		fmt::dynamic_format_arg_store<fmt::format_context> args;

		sol::protected_function tostring = lua["tostring"];

		auto translate = [&](auto& func, auto& var) {
			sol::protected_function_result ret = func(var);
			if (!ret.valid()) {
				args.push_back("__tostring error!");
				return;
			}

			args.push_back(ret.get<std::string>());
		};

		for (auto& var : vars) {
			if (var.get_type() == sol::type::table) {
				sol::protected_function tblStrFunc = var.as<sol::table>()["__tostring"];
				if (!tblStrFunc.valid()) {
					translate(tostring, var);
					continue;
				}

				translate(tblStrFunc, var);
			} else {
				translate(tostring, var);
			}
		}

		auto outp = fmt::vformat(str, args);
		fmt::print("{}\n", outp);
	}

	void Script::callFunc(const StreamDeckMessage& msg, const std::string& name) {
		sol::table tbl = lua["script"];
		sol::protected_function func = tbl[name];

		if (!func.valid()) return;
		sol::protected_function_result result = func(tbl, ButtonWrapper(msg));
		if (result.valid()) return;

		sol::error err = result;
		std::string errStr = err.what();
		replaceAll(errStr, folder, "");
		fmt::print("[LUA ERROR] {}\n", errStr);
	}

	void Script::onWillAppear(const StreamDeckMessage& msg) { callFunc(msg, "onWillAppear"); }
	void Script::onWillDisappear(const StreamDeckMessage& msg){ callFunc(msg, "onWillDisappear"); }
	void Script::onPressDown(const StreamDeckMessage& msg) { callFunc(msg, "onPressDown"); }
	void Script::onPressUp(const StreamDeckMessage& msg) { callFunc(msg, "onPressUp"); }

	void Script::setDeck(StreamDeck& deck) {
		lua["deck"] = DeckWrapper(deck);
	}
 }
