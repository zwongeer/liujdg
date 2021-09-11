FILE(GLOB libliujdg_deamon_files *.cxx *.hpp)
add_library(libliujdg_deamon STATIC ${libliujdg_deamon_files})
find_package(Boost COMPONENTS chrono thread system REQUIRED) 
target_include_directories(libliujdg_deamon
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
		$<INSTALL_INTERFACE:include>
		${CMAKE_CURRENT_BINARY_DIR}/../../generated
)
target_link_libraries(libliujdg_deamon ${Boost_LIBRARIES})