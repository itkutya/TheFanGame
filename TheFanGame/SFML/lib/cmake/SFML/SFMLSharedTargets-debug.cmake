#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SFML::System" for configuration "Debug"
set_property(TARGET SFML::System APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SFML::System PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/sfml-system-d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/sfml-system-d-3.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS SFML::System )
list(APPEND _IMPORT_CHECK_FILES_FOR_SFML::System "${_IMPORT_PREFIX}/lib/sfml-system-d.lib" "${_IMPORT_PREFIX}/bin/sfml-system-d-3.dll" )

# Import target "SFML::Main" for configuration "Debug"
set_property(TARGET SFML::Main APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SFML::Main PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/sfml-main-d.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS SFML::Main )
list(APPEND _IMPORT_CHECK_FILES_FOR_SFML::Main "${_IMPORT_PREFIX}/lib/sfml-main-d.lib" )

# Import target "SFML::Window" for configuration "Debug"
set_property(TARGET SFML::Window APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SFML::Window PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/sfml-window-d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/sfml-window-d-3.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS SFML::Window )
list(APPEND _IMPORT_CHECK_FILES_FOR_SFML::Window "${_IMPORT_PREFIX}/lib/sfml-window-d.lib" "${_IMPORT_PREFIX}/bin/sfml-window-d-3.dll" )

# Import target "SFML::Network" for configuration "Debug"
set_property(TARGET SFML::Network APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SFML::Network PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/sfml-network-d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/sfml-network-d-3.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS SFML::Network )
list(APPEND _IMPORT_CHECK_FILES_FOR_SFML::Network "${_IMPORT_PREFIX}/lib/sfml-network-d.lib" "${_IMPORT_PREFIX}/bin/sfml-network-d-3.dll" )

# Import target "SFML::Graphics" for configuration "Debug"
set_property(TARGET SFML::Graphics APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SFML::Graphics PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/sfml-graphics-d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/sfml-graphics-d-3.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS SFML::Graphics )
list(APPEND _IMPORT_CHECK_FILES_FOR_SFML::Graphics "${_IMPORT_PREFIX}/lib/sfml-graphics-d.lib" "${_IMPORT_PREFIX}/bin/sfml-graphics-d-3.dll" )

# Import target "SFML::Audio" for configuration "Debug"
set_property(TARGET SFML::Audio APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SFML::Audio PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/sfml-audio-d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/sfml-audio-d-3.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS SFML::Audio )
list(APPEND _IMPORT_CHECK_FILES_FOR_SFML::Audio "${_IMPORT_PREFIX}/lib/sfml-audio-d.lib" "${_IMPORT_PREFIX}/bin/sfml-audio-d-3.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
