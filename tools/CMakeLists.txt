add_executable(liujdg.true liujdg.true.cxx)
target_link_libraries(liujdg.true libliujdg)
target_include_directories(liujdg.true PUBLIC
"${PROJECT_BINARY_DIR}"
"${PROJECT_SOURCE_DIR}/include"
)

add_executable(liujdg.test liujdg.test.cxx)
target_link_libraries(liujdg.test libliujdg)
target_include_directories(liujdg.test PUBLIC
"${PROJECT_BINARY_DIR}"
"${PROJECT_SOURCE_DIR}/include"
)

install(FILES "liujdg.uninstall.sh"
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    DESTINATION "bin"
    RENAME "liujdg.uninstall")