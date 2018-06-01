# Copyright (c) 2012-2018 The Elastos Open Source Project
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

set(SAMPLE_NAME sample)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SAMPLE_SOURCE_FILES)
add_executable(${SAMPLE_NAME} ${SAMPLE_SOURCE_FILES})
target_link_libraries(${SAMPLE_NAME} idchain dl leveldb ${Boost_LIBRARIES})
add_dependencies(${SAMPLE_NAME} idchain leveldb)