
add_subdirectory(Elastos.ELA.SPV.Cpp)

include(Elastos.ELA.SPV.Cpp.cmake)

set(
	ThirdParty_INCLUDE_DIRS
	${ThirdParty_SPVSDK_INC_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Elastos.ELA.SPV.Cpp
	CACHE INTERNAL "ThirdParty include directories" FORCE
)