function(add_file_cxx exec_name)
    set(I 1)
    math(EXPR MAX "${ARGC}")
    set(LIBS)
    while(I LESS ${MAX})
        set(LIBS ${LIBS} ${ARGV${I}})
        math(EXPR I "${I} + 1")
    endwhile()

    add_executable(${exec_name} ${exec_name}.cxx)
    # message("Name = " &{exec_name})
    # message("LINK LIBS = " ${LIBS})
    target_link_libraries(${exec_name} PRIVATE ${LIBS})
    # target_include_directories(${exec_name} PRIVATE
    #     "${PROJECT_BINARY_DIR}"
    #     "${PROJECT_SOURCE_DIR}/include"
    # )
endfunction()

add_file_cxx(liujdg.true)
find_package(fmt REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(Boost COMPONENTS chrono filesystem thread system random REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
set(LIBS libliujdg ${Boost_LIBRARIES} fmt::fmt)
add_file_cxx(liujdg.test ${LIBS})
add_file_cxx(liujdg.build ${LIBS})
add_file_cxx(liujdg.run ${LIBS})

add_subdirectory(deamon)
set(LIBS libliujdg_deamon bcrypt CURL::libcurl libliujdg)
add_file_cxx(liujdg.deamon ${LIBS})

install(FILES "liujdg.uninstall.sh"
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    DESTINATION "bin"
    RENAME "liujdg.uninstall")