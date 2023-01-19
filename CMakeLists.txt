cmake_minimum_required(VERSION 3.21)

project(scrollwheelfix-NG VERSION 1.0.0)

find_package(SkyrimScripting.CMake CONFIG REQUIRED)
find_package(SkyrimScripting.Plugin CONFIG REQUIRED)


# --- source files ---

set(MYHEADERS
	src/Hooks.h
)

set(MYSOURCES
	src/Hooks.cpp
	src/main.cpp
)



add_skse_plugin(${PROJECT_NAME} SOURCES ${MYHEADERS} ${MYSOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE SkyrimScriptingPlugin::SkyrimScripting.Plugin)
