CMAKE_MINIMUM_REQUIRED (VERSION 3.10)

project(IFCPP)
message(STATUS "------------------------------------------------------------------------")

option(USE_OSG_DEBUG "Use openscenegraph debug library" OFF)
option(BUILD_STATIC_LIBRARY "Build static library" OFF)

IF(NOT WIN32)
    IF("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
        IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            SET_PROPERTY(DIRECTORY APPEND PROPERTY
                COMPILE_DEFINITIONS $<$<CONFIG:Debug>:_DEBUG>
            )
        ENDIF()
    ENDIF()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
ELSE(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
ENDIF(NOT WIN32)

#set(CARVE_SOURCE_DIR ${IFCPP_SOURCE_DIR}/external/Carve)
#set(CARVE_BINARY_DIR ${IFCPP_BINARY_DIR})
#set(IFCPP_CONFIG_DIR "share/IFCPP/cmake")
#ADD_SUBDIRECTORY (external/Carve)
ADD_SUBDIRECTORY (IfcPlusPlus)
ADD_SUBDIRECTORY (external/c3dkernel)
ADD_SUBDIRECTORY (examples/LoadFileWithGeometryExampleConsole)

# Install configuration file
#INCLUDE(CMakePackageConfigHelpers)
#set(config_file_input  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/IFCPPConfig.cmake.in")
#set(config_file_output "${CMAKE_CURRENT_BINARY_DIR}/cmake/IFCPPConfig.cmake")

#CONFIGURE_PACKAGE_CONFIG_FILE(
#  ${config_file_input}
#  ${config_file_output}
#  INSTALL_DESTINATION ${IFCPP_CONFIG_DIR})

#INSTALL(
#  FILES ${config_file_output}
#  DESTINATION ${IFCPP_CONFIG_DIR})

