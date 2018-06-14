#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libTrodesNetwork::TrodesNetwork" for configuration "Release"
set_property(TARGET libTrodesNetwork::TrodesNetwork APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libTrodesNetwork::TrodesNetwork PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libTrodesNetwork.so.0.1.0"
  IMPORTED_SONAME_RELEASE "libTrodesNetwork.so.0.1.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS libTrodesNetwork::TrodesNetwork )
list(APPEND _IMPORT_CHECK_FILES_FOR_libTrodesNetwork::TrodesNetwork "${_IMPORT_PREFIX}/lib/libTrodesNetwork.so.0.1.0" )

# Import target "libTrodesNetwork::trodesnetwork" for configuration "Release"
set_property(TARGET libTrodesNetwork::trodesnetwork APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libTrodesNetwork::trodesnetwork PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/spikegadgets_python/spikegadgets/trodesnetwork.so.0.1.0"
  IMPORTED_SONAME_RELEASE "trodesnetwork.so.0.1.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS libTrodesNetwork::trodesnetwork )
list(APPEND _IMPORT_CHECK_FILES_FOR_libTrodesNetwork::trodesnetwork "${_IMPORT_PREFIX}/spikegadgets_python/spikegadgets/trodesnetwork.so.0.1.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
