FILE(GLOB libliujdg_deamon_files *.cxx *.hpp)
add_library(libliujdg_deamon STATIC ${libliujdg_deamon_files})
find_package(Boost COMPONENTS chrono filesystem thread system REQUIRED)
find_package(unofficial-http-parser REQUIRED)
find_package(restinio CONFIG REQUIRED)
find_package(SOCI CONFIG REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_include_directories(libliujdg_deamon
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
		$<INSTALL_INTERFACE:include>
		${CMAKE_CURRENT_BINARY_DIR}/../../generated
)
target_link_libraries(libliujdg_deamon PRIVATE SOCI::soci_sqlite3 SOCI::soci_core SOCI::soci_empty restinio::restinio)