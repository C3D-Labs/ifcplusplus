cmake_minimum_required (VERSION 3.1)

set(TARGET_NAME web-converter)
project(TARGET_NAME)

IF(NOT WIN32)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
ELSE(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
ENDIF(NOT WIN32)

add_subdirectory (IfcPlusPlus)
add_subdirectory (api-client)
add_subdirectory (external/c3dkernel)

add_executable(${TARGET_NAME} src/main.cpp src/base64.cpp)

target_link_libraries(${TARGET_NAME} PRIVATE 
    IfcPlusPlus
    c3dservice-api
    boost_system
    c3d
    pthread
    )
