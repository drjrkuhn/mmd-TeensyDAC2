set(MM_MMCORE_INCLUDEDIR "${MM_SRCROOT}/MMCore")

set(MM_CORE_DEFINITIONS _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS WIN32 _DEBUG _LIB _WINDOWS)

set(MM_CORE_SRC
    CircularBuffer.cpp
    Configuration.cpp
    CoreCallback.cpp
    CoreProperty.cpp
    DeviceManager.cpp
    Devices/AutoFocusInstance.cpp
    Devices/CameraInstance.cpp
    Devices/DeviceInstance.cpp
    Devices/GalvoInstance.cpp
    Devices/HubInstance.cpp
    Devices/ImageProcessorInstance.cpp
    Devices/MagnifierInstance.cpp
    Devices/SerialInstance.cpp
    Devices/ShutterInstance.cpp
    Devices/SignalIOInstance.cpp
    Devices/SLMInstance.cpp
    Devices/StageInstance.cpp
    Devices/StateInstance.cpp
    Devices/XYStageInstance.cpp
    Error.cpp
    FrameBuffer.cpp
    Host.cpp
    LibraryInfo/LibraryPathsWindows.cpp
    LoadableModules/LoadedDeviceAdapter.cpp
    LoadableModules/LoadedModule.cpp
    LoadableModules/LoadedModuleImpl.cpp
    LoadableModules/LoadedModuleImplWindows.cpp
    Logging/Metadata.cpp
    LogManager.cpp
    MMCore.cpp
    PluginManager.cpp
    Semaphore.cpp
    Task.cpp
    TaskSet.cpp
    TaskSet_CopyMemory.cpp
    ThreadPool.cpp

    CircularBuffer.h
    ConfigGroup.h
    Configuration.h
    CoreCallback.h
    CoreProperty.h
    CoreUtils.h
    DeviceManager.h
    Devices/AutoFocusInstance.h
    Devices/CameraInstance.h
    Devices/DeviceInstance.h
    Devices/DeviceInstanceBase.h
    Devices/DeviceInstances.h
    Devices/GalvoInstance.h
    Devices/GenericInstance.h
    Devices/HubInstance.h
    Devices/ImageProcessorInstance.h
    Devices/MagnifierInstance.h
    Devices/SerialInstance.h
    Devices/ShutterInstance.h
    Devices/SignalIOInstance.h
    Devices/SLMInstance.h
    Devices/StageInstance.h
    Devices/StateInstance.h
    Devices/XYStageInstance.h
    Error.h
    FrameBuffer.h
    Host.h
    LibraryInfo/LibraryPaths.h
    LoadableModules/LoadedDeviceAdapter.h
    LoadableModules/LoadedModule.h
    LoadableModules/LoadedModuleImpl.h
    LoadableModules/LoadedModuleImplWindows.h
    Logging/GenericEntryFilter.h
    Logging/GenericLinePacket.h
    Logging/GenericLogger.h
    Logging/GenericLoggingCore.h
    Logging/GenericMetadata.h
    Logging/GenericPacketArray.h
    Logging/GenericPacketQueue.h
    Logging/GenericSink.h
    Logging/GenericStreamSink.h
    Logging/Logger.h
    Logging/Logging.h
    Logging/Metadata.h
    Logging/MetadataFormatter.h
    LogManager.h
    MMCore.h
    MMEventCallback.h
    PluginManager.h
    Semaphore.h
    Task.h
    TaskSet.h
    TaskSet_CopyMemory.h
    ThreadPool.h
)
list(TRANSFORM MM_CORE_SRC PREPEND ${MM_MMCORE_INCLUDEDIR}/)

add_library(MMCore STATIC ${MM_CORE_SRC})
target_compile_definitions(MMCore PUBLIC ${MM_CORE_DEFINITIONS})
target_include_directories(MMCore PUBLIC ${MM_MMCORE_INCLUDEDIR} ${MM_MMDEVICE_INCLUDEDIR})
target_compile_features(MMCore PUBLIC cxx_std_14)
target_link_libraries(MMCore PRIVATE Iphlpapi.lib Boost::boost Boost::thread)
set_target_properties(MMCore
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)
