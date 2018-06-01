# Copyright (c) 2012-2018 The Elastos Open Source Project
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

set(
	SDK_INC_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}/Implement
	${CMAKE_CURRENT_SOURCE_DIR}/Database
	${CMAKE_CURRENT_SOURCE_DIR}
	CACHE INTERNAL "include directories" FORCE
)

file(GLOB_RECURSE SDK_IMPL_SOURCE_FILES "*.cpp")

set(
	SDK_IMPL_SOURCE_FILES
	${SDK_IMPL_SOURCE_FILES}
	CACHE INTERNAL "source files" FORCE
)
