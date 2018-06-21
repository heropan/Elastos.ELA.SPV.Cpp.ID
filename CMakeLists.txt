# Copyright (c) 2012-2018 The Elastos Open Source Project
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define helper macro option_with_default
macro(option_with_default OPTION_NAME OPTION_STRING OPTION_DEFAULT)
	if(NOT DEFINED ${OPTION_NAME})
		set(${OPTION_NAME} ${OPTION_DEFAULT})
	endif(NOT DEFINED ${OPTION_NAME})
	option(${OPTION_NAME} "${OPTION_STRING}" ${${OPTION_NAME}})
endmacro(option_with_default OPTION_NAME OPTION_STRING OPTION_DEFAULT)

option_with_default(IDCHAIN_FOR_ANDROID "Build spv for android" OFF)

if(IDCHAIN_FOR_ANDROID)
	set(SPV_FOR_ANDROID ON)
	message(STATUS "Build idchain for android")

	set(CMAKE_SYSTEM_NAME Android)
	set(CMAKE_ANDROID_STL_TYPE c++_static)
	#set(CMAKE_ANDROID_STL_TYPE c++_shared)

	if(NOT DEFINED ANDROID_NATIVE_API_LEVEL)
		set(CMAKE_SYSTEM_VERSION 23)
	else()
		set(CMAKE_SYSTEM_VERSION ${ANDROID_NATIVE_API_LEVEL})
	endif()

	if("$ENV{ANDROID_NDK}" STREQUAL "")
		message(FATAL_ERROR "Environment variable ANDROID_NDK not define")
	endif()
	set(CMAKE_ANDROID_NDK $ENV{ANDROID_NDK})
	set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION clang)

	# can be value below: arm64-v8a armeabi-v7a armeabi-v6 armeabi x86 x86_64
	if(NOT DEFINED CMAKE_ANDROID_ARCH_ABI)
		set(CMAKE_ANDROID_ARCH_ABI armeabi-v7a CACHE STRING "Android ABI (architecture)" FORCE)
	endif()
	set_property(CACHE CMAKE_ANDROID_ARCH_ABI PROPERTY STRINGS arm64-v8a armeabi-v7a armeabi-v6 armeabi x86 x86_64)
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type." FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release Debug RelWithDebInfo MinSizeRel)

project(idchain)

#set(CMAKE_VERBOSE_MAKEFILE ON)

option_with_default(IDCHAIN_BUILD_TEST_CASES "Build test cases" ON)
option_with_default(IDCHAIN_BUILD_SAMPLE "Build sample" ON)

set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_RELEASE NDEBUG)
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_MINSIZEREL NDEBUG)
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO NDEBUG)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DSPDLOG_DEBUG_ON)
endif()

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
		SDK/Implement/did.h SDK/Implement/did.cpp Interface/ididChecker.h Interface/idid.h Interface/ididManager.h SDK/Implement/didManager.h SDK/Implement/didManager.cpp SDK/Implement/didChecker.cpp SDK/Implement/didChecker.h)

target_link_libraries(idchain spvsdk leveldb)
add_dependencies(idchain spvsdk leveldb)

if(IDCHAIN_BUILD_SAMPLE)
	add_subdirectory(Sample)
endif()

if(IDCHAIN_BUILD_TEST_CASES)
	add_subdirectory(Test)
endif()
