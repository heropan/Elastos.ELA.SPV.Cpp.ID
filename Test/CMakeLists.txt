# Copyright (c) 2012-2018 The Elastos Open Source Project
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} IDTEST_SOURCE_FILES)
foreach(src ${IDTEST_SOURCE_FILES})
	string(REGEX REPLACE ".*/\(.*\).cpp$" "\\1" IDTEST_TARGET_NAME ${src})
	add_executable(${IDTEST_TARGET_NAME} ${src})
	target_link_libraries(${IDTEST_TARGET_NAME} idchain dl leveldb ${Boost_LIBRARIES})
	add_dependencies(${IDTEST_TARGET_NAME} idchain leveldb)
endforeach()