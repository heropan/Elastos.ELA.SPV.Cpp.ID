# Copyright (c) 2012-2018 The Elastos Open Source Project
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

add_custom_command(
	COMMENT "Copy Data to IdSample"
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Data
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/Elastos.ELA.SPV.Cpp/Data ${CMAKE_CURRENT_BINARY_DIR}/Data
	)
add_custom_target(id_copy_sample_data DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Data)

set(ID_SAMPLE_NAME IdSample)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SAMPLE_SOURCE_FILES)
add_executable(${ID_SAMPLE_NAME} ${SAMPLE_SOURCE_FILES})
target_link_libraries(${ID_SAMPLE_NAME} idchain dl leveldb ${Boost_LIBRARIES})
add_dependencies(${ID_SAMPLE_NAME} idchain leveldb)
add_dependencies(${ID_SAMPLE_NAME} id_copy_sample_data)
