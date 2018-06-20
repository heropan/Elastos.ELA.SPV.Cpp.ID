# Copyright (c) 2012-2018 The Elastos Open Source Project
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

add_custom_target(
	idchain_auto_test ALL
)

unset(AUTO_TEST_COMMAND)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} IDTEST_SOURCE_FILES)
foreach(src ${IDTEST_SOURCE_FILES})
	string(REGEX REPLACE ".*/\(.*\).cpp$" "\\1" IDTEST_TARGET_NAME ${src})
	add_executable(${IDTEST_TARGET_NAME} ${src})
	target_link_libraries(${IDTEST_TARGET_NAME} idchain dl leveldb ${Boost_LIBRARIES})
	add_dependencies(${IDTEST_TARGET_NAME} idchain leveldb)

	if("${AUTO_TEST_COMMAND}" STREQUAL "")
		set(AUTO_TEST_COMMAND ./${IDTEST_TARGET_NAME})
	else()
		set(AUTO_TEST_COMMAND ${AUTO_TEST_COMMAND} && ./${IDTEST_TARGET_NAME})
	endif()
	add_dependencies(idchain_auto_test ${IDTEST_TARGET_NAME})

	if(ANDROID)
		target_link_libraries(${IDTEST_TARGET_NAME} log atomic)
	endif()
endforeach()

if(NOT IDCHAIN_FOR_ANDROID)
	add_custom_command(
		TARGET idchain_auto_test
		POST_BUILD
		COMMAND ${AUTO_TEST_COMMAND}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)
endif()
