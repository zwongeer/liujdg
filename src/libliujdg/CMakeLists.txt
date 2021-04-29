add_library(libliujdg STATIC
    Lfunc.cxx
    LGame.cxx
    LGameInfo.cxx
    LGameResult.cxx
    Ljdgconfig.cxx
    LPerson.cxx
    LPlayerResult.cxx
    LProcess_container.cxx
    LProcess.cxx
)
target_link_libraries(libliujdg -lm pthread stdc++fs)

# This is only necessary for older compilers, but doesn't hurt
set_target_properties(libliujdg PROPERTIES POSITION_INDEPENDENT_CODE 1)