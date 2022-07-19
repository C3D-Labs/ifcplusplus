cmake_minimum_required (VERSION 3.1)

set(TARGET_NAME web-converter)
project(TARGET_NAME)

IF(NOT WIN32)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
ELSE(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
ENDIF(NOT WIN32)

add_subdirectory (IfcPlusPlus)
#add_subdirectory(cpp-api-client)

add_executable(${TARGET_NAME} src/main.cpp)

target_link_libraries(${TARGET_NAME} PRIVATE 
    IfcPlusPlus
    #c3dservice-api
    #boost_system
    )
