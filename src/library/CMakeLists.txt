set(LIBRARY_SOURCES
    "library.cc"
    )
set(LIBRARY_HEADERS
    "libapi.hpp"
    "stdafx.hpp"
    )
    option(WIN_MACRO "Build Windows DLL" ON) # Enabled by default
    if(WIN_MACRO)
        add_compile_definitions(-D_WIN64 -D_DLL)
    endif(WIN_MACRO)
add_library(${LIBRARY_NAME} SHARED
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS})
target_include_directories(${LIBRARY_NAME} PUBLIC
    "./")
install(TARGETS ${LIBRARY_NAME} CONFIGURATIONS Debug DESTINATION Debug)