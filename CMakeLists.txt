# Top-level CMake build file for the `liujdg` program

# This file is part of the "liujdg" project

# CMake required version
cmake_minimum_required(VERSION 3.10)

project(liujdg VERSION "0.0.1")

# library name
set(LIBLIUJDG libliujdg)
set(LIBLIUJDG_STATIC ${LIBLIUJDG}-static)

# Use c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(WIN32)
	add_compile_options("-D_WIN32_WINNT=0x0601")
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/liujdg_version.hpp.in 
    generated/${LIUJDG}/liujdg_version.hpp
    @ONLY
)

add_subdirectory(src/libliujdg)

target_include_directories(libliujdg
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		${CMAKE_CURRENT_BINARY_DIR}/generated
	PRIVATE
		src/libliujdg
)

add_executable(liujdg.install liujdg.install.cxx)

find_package(Boost COMPONENTS filesystem REQUIRED)
target_link_libraries(liujdg.install PRIVATE libliujdg ${Boost_LIBRARIES})

# install(TARGETS liujdg.run
# 		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
# 		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
# 		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_subdirectory(tools)