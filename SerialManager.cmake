set(MM_SERIALMANAGER_INCLUDEDIR "${MM_SRCROOT}/DeviceAdapters/SerialManager")

set (MM_SERIALMANAGER_DEFINITIONS ${MMDEVICE_DEFINITIONS})

set(MM_SERIALMANAGER_SRC 
    SerialManager.cpp
    SerialManager.h
    AsioClient.h
)
list(TRANSFORM MM_SERIALMANAGER_SRC PREPEND ${MM_SERIALMANAGER_INCLUDEDIR}/)

add_library(mmgr_dal_SerialManager SHARED ${MM_SERIALMANAGER_SRC})
target_compile_definitions(mmgr_dal_SerialManager 
    PUBLIC ${MM_SERIALMANAGER_DEFINITIONS}
)
target_include_directories(mmgr_dal_SerialManager 
    PUBLIC ${MM_MMDEVICE_INCLUDEDIR} 
    PRIVATE ../lib/ArduinoJson/src ../lib/Ardulingua/src
)
target_link_libraries(mmgr_dal_SerialManager MMDevice-Shared Boost::boost)
set_target_properties(mmgr_dal_SerialManager
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)
