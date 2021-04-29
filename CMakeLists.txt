# Top-level CMake build file for the `liujdg` program

# This file is part of the "liujdg" project

# CMake required version
cmake_minimum_required(VERSION 3.10)

project(liujdg.run VERSION "0.0.1")

# library name
set(LIBLIUJDG libliujdg)
set(LIBLIUJDG_STATIC ${LIBLIUJDG}-static)

# Use c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
	PRIVATE 
		${CMAKE_CURRENT_BINARY_DIR}/generated
		src/libliujdg
)

# liujdg main program
add_executable(liujdg.run liujdg.run.cxx)

target_link_libraries(liujdg.run libliujdg)
target_include_directories(liujdg.run PUBLIC
"${PROJECT_BINARY_DIR}"
"${PROJECT_SOURCE_DIR}/include"
)

add_executable(liujdg.install liujdg.install.cxx)

target_link_libraries(liujdg.install libliujdg)
                          
target_include_directories(liujdg.install PUBLIC
"${PROJECT_BINARY_DIR}"
"${PROJECT_SOURCE_DIR}/include"
)

# install(TARGETS liujdg.run
# 		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
# 		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
# 		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_subdirectory(tools)