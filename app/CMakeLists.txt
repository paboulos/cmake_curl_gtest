set(EXE_SOURCES
    "bootstrap.cc")

add_library( ${EXE_LIB_NAME} STATIC run.cc)
target_include_directories(
    ${EXE_LIB_NAME} 
    PUBLIC 
    .
    ${CURL_INCLUDE_DIRS})
target_link_libraries(
    ${EXE_LIB_NAME} 
    PUBLIC
    ${LIBRARY_NAME}
    ${CURL_LIBRARIES})
add_executable(${EXECUTABLE_NAME} ${EXE_SOURCES})
target_link_libraries(
    ${EXECUTABLE_NAME} 
    PUBLIC
    ${EXE_LIB_NAME})

install(TARGETS ${EXECUTABLE_NAME} CONFIGURATIONS Debug RUNTIME DESTINATION Debug)

message(CMAKE_BINARY_DIR= "${CMAKE_BINARY_DIR}")
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_17)