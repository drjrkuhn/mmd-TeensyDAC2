set(MM_MMDEVICE_INCLUDEDIR "${MM_SRCROOT}/MMDevice")

set(MM_DEVICE_DEFINITIONS _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS WIN32 NDEBUG _WINDOWS _USRDLL MODULE_EXPORTS)

set(MM_DEVICE_SRC 
    Debayer.cpp
    DeviceUtils.cpp
    ImgBuffer.cpp
    MMDevice.cpp
    ModuleInterface.cpp
    Property.cpp
    Debayer.h
    DeviceBase.h
    DeviceUtils.h
    FixSnprintf.h
    ImageMetadata.h
    ImgBuffer.h
    MMDevice.h
    MMDeviceConstants.h
    ModuleInterface.h
    Property.h
)
list(TRANSFORM MM_DEVICE_SRC PREPEND ${MM_MMDEVICE_INCLUDEDIR}/)

add_library(MMDevice-Shared ${MM_DEVICE_SRC})
target_include_directories(MMDevice-Shared PRIVATE ${MM_MMDEVICE_INCLUDEDIR})
target_compile_definitions(MMDevice-Shared PUBLIC ${MM_DEVICE_DEFINITIONS})
target_link_libraries(MMDevice-Shared Boost::boost Boost::thread)
set_target_properties(MMDevice-Shared
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)
