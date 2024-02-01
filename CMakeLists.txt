cmake_minimum_required (VERSION 3.8)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin/${CMAKE_BUILD_TYPE})

include("cmake/extlibs.cmake")

project ("RawrStreamDeck" VERSION "0.0.1")

if (WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRTD")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='$ORIGIN'")
endif()

file(GLOB_RECURSE source_files
	"code/src/*.cpp"
	"code/src/*.c"
	"code/include/*.hpp"
	"code/include/*.h"
)

set(output_target RawrStreamDeck)

add_executable(${output_target} ${source_files} "project.rc")
target_include_directories(${output_target} PUBLIC "code/include" ${luajit_SOURCE_DIR}/src)
target_compile_definitions(${output_target} PUBLIC NOMINMAX SOL_LUAJIT=1)
target_compile_features(${output_target} PRIVATE cxx_std_20)
target_link_libraries(${output_target} PRIVATE
	fmt::fmt-header-only
	nlohmann_json
	hv
	curlpp_static
	magic_enum::magic_enum
	liblua-shared
	sol2::sol2
)

if (UNIX)
	target_link_libraries(${output_target} PUBLIC stdc++fs)
endif()

add_custom_target(${output_target}buildbin ALL
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_HOME_DIRECTORY}/build/bin
		${CMAKE_HOME_DIRECTORY}/bin/${CMAKE_BUILD_TYPE}/
		COMMENT "Copying binaries into binary directory")
add_dependencies(${output_target} ${output_target}buildbin)

add_custom_target(${output_target}buildcontent ALL
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_HOME_DIRECTORY}/content
		${CMAKE_HOME_DIRECTORY}/bin/${CMAKE_BUILD_TYPE}/content
		COMMENT "Copying content into binary directory")
add_dependencies(${output_target} ${output_target}buildcontent)

add_custom_target(${output_target}lua ALL
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_HOME_DIRECTORY}/lua
		${CMAKE_HOME_DIRECTORY}/bin/${CMAKE_BUILD_TYPE}/lua
		COMMENT "Copying lua into binary directory")
add_dependencies(${output_target} ${output_target}lua)
