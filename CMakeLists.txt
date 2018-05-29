cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(idchain)

#set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(Interface)
add_subdirectory(SDK)
add_subdirectory(ThirdParty)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${SDK_INC_DIRS}
	${INTERFACE_INC_DIRS}
	${ThirdParty_INCLUDE_DIRS}
)

add_library(
	idchain SHARED
	${SDK_IMPL_SOURCE_FILES}
)

target_link_libraries(idchain spvsdk)
add_dependencies(idchain spvsdk)

add_subdirectory(Sample)