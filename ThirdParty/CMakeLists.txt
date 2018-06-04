# Copyright (c) 2012-2018 The Elastos Open Source Project
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

add_subdirectory(Elastos.ELA.SPV.Cpp)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(LEVELDB_BUILD_TESTS OFF)
add_subdirectory(leveldb)

include(Elastos.ELA.SPV.Cpp.cmake)
include(leveldb.cmake)

set(
	ThirdParty_INCLUDE_DIRS
	${ThirdParty_SPVSDK_INC_DIR}
	${ThirdParty_LEVELDB_INC_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Elastos.ELA.SPV.Cpp
	CACHE INTERNAL "ThirdParty include directories" FORCE
)