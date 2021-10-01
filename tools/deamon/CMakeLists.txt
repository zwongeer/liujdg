FILE(GLOB libliujdg_deamon_files *.cxx *.hpp)
add_library(libliujdg_deamon STATIC ${libliujdg_deamon_files})
find_package(Boost COMPONENTS chrono filesystem thread system REQUIRED)

if(WIN32)
	find_package(unofficial-http-parser REQUIRED)
endif()

find_package(restinio CONFIG REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_include_directories(libliujdg_deamon
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
		$<INSTALL_INTERFACE:include>
		${CMAKE_CURRENT_BINARY_DIR}/../../generated
)

if(WIN32)
	target_link_libraries(libliujdg_deamon bcrypt soci_core_4_0 libsoci_empty_4_0 libsoci_sqlite3_4_0 sqlite3 curl restinio::restinio ${Boost_LIBRARIES})
else()
	target_link_libraries(libliujdg_deamon soci_sqlite3 soci_core curl restinio::restinio ${Boost_LIBRARIES})
endif()