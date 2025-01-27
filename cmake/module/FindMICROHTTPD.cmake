function(add_imported_library library headers)
  add_library(MICROHTTPD::MICROHTTPD STATIC IMPORTED)
  set_target_properties(MICROHTTPD::MICROHTTPD PROPERTIES
    IMPORTED_LOCATION ${library}
    INTERFACE_INCLUDE_DIRECTORIES ${headers}
  )
  set(MICROHTTPD_FOUND TRUE CACHE BOOL "MICROHTTPD found" FORCE)
  set(MICROHTTPD_LIBRARIES ${library}
      CACHE STRING "Path to MICROHTTPD library" FORCE)
  set(MICROHTTPD_INCLUDES ${headers}
      CACHE STRING "Path to MICROHTTPD headers" FORCE)
  mark_as_advanced(FORCE MICROHTTPD_LIBRARIES)
  mark_as_advanced(FORCE MICROHTTPD_INCLUDES)
endfunction()

if (MICROHTTPD_LIBRARIES AND MICROHTTPD_INCLUDES)
  add_imported_library(${MICROHTTPD_LIBRARIES} ${MICROHTTPD_INCLUDES})
  return()
endif()