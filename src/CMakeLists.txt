cmake_minimum_required(VERSION 3.8.0)
project(TeensyDAC2 VERSION 0.1.0)

#MESSAGE(STATUS "${PROJECT_NAME} folder CMakeLists.txt")
#get_filename_component(MM_MMDEVICE_INCLUDEDIR_ABSOLUTE_PATH "${MM_MMDEVICE_INCLUDEDIR}" ABSOLUTE)
#MESSAGE(STATUS "MM_MMDEVICE_INCLUDEDIR:   " ${MM_MMDEVICE_INCLUDEDIR_ABSOLUTE_PATH} )

set(TARGET_NAME "mmgr_dal_${PROJECT_NAME}")

add_library(${TARGET_NAME} SHARED TeensyDAC.cpp TeensyDAC.h)
target_compile_definitions(${TARGET_NAME} PUBLIC ${MM_DEVICE_DEFINITIONS})
target_include_directories(${TARGET_NAME} PRIVATE ${MM_MMDEVICE_INCLUDEDIR} ../lib/ArduinoJson/src ../lib/Ardulingua/src)
target_link_libraries(${TARGET_NAME} MMDevice-Shared)
set_target_properties(${TARGET_NAME}
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)

install(TARGETS ${TARGET_NAME} DESTINATION ${MM_INSTALLDIR})