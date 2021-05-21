cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5 REQUIRED COMPONENTS
	Widgets
)

#set(SOURCES
#	WIN32 main.cpp
#)
file(GLOB SOURCES *.cpp)
#qt5_add_resources (RCC_SOURCES rc.qrc)
add_executable(webviewExample ${SOURCES} ${RCC_SOURCES})
target_link_libraries(webviewExample Qt5::Core
	Qt5::Widgets
	#$<$<PLATFORM_ID:Linux>:pthread>
)

