include("cmake/cpm.cmake")

CPMAddPackage("gh:fmtlib/fmt#10.1.1")

CPMAddPackage("gh:Neargye/magic_enum@0.9.3")
CPMAddPackage("gh:ithewei/libhv#master")

add_definitions("-DCURL_STATICLIB")
set(CURL_STATICLIB ON)
set(CURL_USE_SCHANNEL ON)
CPMAddPackage("gh:curl/curl#master")

set(CURL_FOUND ON)
set(CURL_LIBRARIES libcurl)
set(CURL_VERSION_STRING "Who knows, this fucking lib doesn't")
set(CURL_INCLUDE_DIRS ${curl_SOURCE_DIR}/src)
set(CURLPP_BUILD_SHARED_LIBS OFF)
CPMAddPackage("gh:jpbarrette/curlpp#master")

add_custom_target(curlBinaries ALL
		COMMAND ${CMAKE_COMMAND} -E copy
		${curl_BINARY_DIR}/lib/libcurl$<$<CONFIG:Debug>:-d>.dll
		${CMAKE_HOME_DIRECTORY}/bin/${CMAKE_BUILD_TYPE}/
		COMMENT "Copying curl binaries into binary directory")
add_dependencies(curlBinaries curlpp_static)

CPMAddPackage(
    NAME nlohmann_json
    VERSION 3.11.2
    # the git repo is incredibly large, so we download the archived include directory
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/include.zip
    URL_HASH SHA256=e5c7a9f49a16814be27e4ed0ee900ecd0092bfb7dbfca65b5a421b774dccaaed)

if(nlohmann_json_ADDED)
    add_library(nlohmann_json INTERFACE IMPORTED)
    target_include_directories(nlohmann_json INTERFACE ${nlohmann_json_SOURCE_DIR}/include)
endif()
add_compile_definitions(
	"LUAJIT_ENABLE_CHECKHOOK=1"
	"SOL_PRINT_ERRORS=0"
	"SOL_EXCEPTIONS_SAFE_PROPAGATION=1"
)

set(SOL2_LUA_VERSION "LuaJIT" CACHE STRING "" FORCE)
CPMAddPackage(
	NAME luajit
	GITHUB_REPOSITORY edunad/LuaJIT-cmake
	GIT_TAG master
	OPTIONS
		"LUAJIT_DISABLE_BUFFER ON"
		"LUA_USE_APICHECK ON"
			"SOL_EXCEPTIONS_SAFE_PROPAGATION ON")

#add_compile_definitions("LUAJIT_ENABLE_CHECKHOOK")
CPMAddPackage(
	NAME sol2
	GITHUB_REPOSITORY ThePhD/sol2
	VERSION 3.3.0)

add_custom_target(luaBinaries ALL
	COMMAND ${CMAKE_COMMAND} -E copy
	${luajit_BINARY_DIR}/../../liblua.dll
	${CMAKE_HOME_DIRECTORY}/bin/${CMAKE_BUILD_TYPE}/
	COMMENT "Copying lua binaries into binary directory")
add_dependencies(luaBinaries liblua-shared)

