FILE(GLOB libliujdg_files *.cxx)
find_package(Boost COMPONENTS chrono filesystem thread system random REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
add_library(libliujdg STATIC ${libliujdg_files})
target_link_libraries(libliujdg PRIVATE ${Boost_LIBRARIES})

# This is only necessary for older compilers, but doesn't hurt
set_target_properties(libliujdg PROPERTIES POSITION_INDEPENDENT_CODE 1)