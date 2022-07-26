set(TARGET_NAME c3d)
project(${TARGET_NAME} CXX)

cmake_minimum_required(VERSION 3.14)

set(C3D_KERNEL_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Directory with c3d library" )

set(C3D_Kernel_INCLUDE_DIR ${C3D_KERNEL_DIR}/include)
set(C3D_Kernel_LIB_DIR ${C3D_KERNEL_DIR}/lib)

# add c3d from binary
add_library( ${TARGET_NAME} SHARED IMPORTED GLOBAL)

target_include_directories( ${TARGET_NAME}
    INTERFACE ${C3D_Kernel_INCLUDE_DIR}
)

if(UNIX)
    set(C3D_LIB_NAME lib${TARGET_NAME}.so)
else()
    set(C3D_LIB_NAME ${TARGET_NAME}.dll)
endif()

set_target_properties(${TARGET_NAME} PROPERTIES
             IMPORTED_CONFIGURATIONS "RELEASE;DEBUG"
             IMPORTED_LOCATION_RELEASE "${C3D_Kernel_LIB_DIR}/Release/${C3D_LIB_NAME}"
             IMPORTED_LOCATION_DEBUG   "${C3D_Kernel_LIB_DIR}/Debug/${C3D_LIB_NAME}"
             IMPORTED_IMPLIB_RELEASE   "${C3D_Kernel_LIB_DIR}/Release/math.lib"
             IMPORTED_IMPLIB_DEBUG     "${C3D_Kernel_LIB_DIR}/Debug/math.lib"
             )