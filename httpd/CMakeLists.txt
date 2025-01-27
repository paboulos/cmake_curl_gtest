set(MHD_SOURCES
    "runhttpd.cc")
add_executable(${MHD_EXECUTABLE} ${MHD_SOURCES})

target_include_directories(
    ${MHD_EXECUTABLE}  
    PUBLIC 
    .
    ${CURL_INCLUDE_DIRS}
    ${MICROHTTPD_INCLUDES})

if( WIN_64 OR WIN_32 )
	set_target_properties(${MHD_EXECUTABLE}
		PROPERTIES 
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
	)
endif()

# MSVC single threaded system libraries use this option
# SET_TARGET_PROPERTIES(${MHD_EXECUTABLE} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:\"LIBCMTD\"")
target_link_libraries(
    ${MHD_EXECUTABLE} 
    PUBLIC
    ${CURL_LIBRARIES}
    ${MICROHTTPD_LIBRARIES})

install(TARGETS ${MHD_EXECUTABLE} CONFIGURATIONS Debug RUNTIME DESTINATION Debug)

target_compile_features(${MHD_EXECUTABLE} PUBLIC cxx_std_17)