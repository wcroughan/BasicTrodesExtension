set(libTrodesNetwork_VERSION 0.1.0)


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was libTrodesNetworkConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

####################################################################################



if(NOT TARGET libTrodesNetwork::TrodesNetwork)
  include("${CMAKE_CURRENT_LIST_DIR}/libTrodesNetworkTargets.cmake")
endif()

# Compatibility
get_property(libTrodesNetwork_TrodesNetwork_INCLUDE_DIR TARGET libTrodesNetwork::TrodesNetwork PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
get_property(libTrodesNetwork_trodesnetwork_INCLUDE_DIR TARGET libTrodesNetwork::trodesnetwork PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

set(libTrodesNetwork_LIBRARIES libTrodesNetwork::TrodesNetwork libTrodesNetwork::trodesnetwork)
set(libTrodesNetwork_INCLUDE_DIRS "${libTrodesNetwork_TrodesNetwork_INCLUDE_DIR}" "${libTrodesNetwork_trodesnetwork_INCLUDE_DIR}")
list(REMOVE_DUPLICATES libTrodesNetwork_INCLUDE_DIRS)
