FILE(GLOB libliujdg_files *.cxx)
find_package(fmt REQUIRED)
add_library(libliujdg STATIC ${libliujdg_files})
target_link_libraries(libliujdg fmt -lm pthread stdc++fs)

# This is only necessary for older compilers, but doesn't hurt
set_target_properties(libliujdg PROPERTIES POSITION_INDEPENDENT_CODE 1)