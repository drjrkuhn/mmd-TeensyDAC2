cmake_minimum_required(VERSION 3.8.0)
project("TeensyDAC2-Test" VERSION 0.1.0)

set(MMDEVICE_DEFINITIONS _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS WIN32 NDEBUG _WINDOWS _USRDLL MODULE_EXPORTS)
set(MMCORE_DEFINITIONS _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS WIN32 _DEBUG _LIB _WINDOWS)

set(MMCORE_SRC
    ${MM_MMCORE_INCLUDEDIR}/CircularBuffer.cpp
    ${MM_MMCORE_INCLUDEDIR}/Configuration.cpp
    ${MM_MMCORE_INCLUDEDIR}/CoreCallback.cpp
    ${MM_MMCORE_INCLUDEDIR}/CoreProperty.cpp
    ${MM_MMCORE_INCLUDEDIR}/DeviceManager.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/AutoFocusInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/CameraInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/DeviceInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/GalvoInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/HubInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/ImageProcessorInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/MagnifierInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/SerialInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/ShutterInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/SignalIOInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/SLMInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/StageInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/StateInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Devices/XYStageInstance.cpp
    ${MM_MMCORE_INCLUDEDIR}/Error.cpp
    ${MM_MMCORE_INCLUDEDIR}/FrameBuffer.cpp
    ${MM_MMCORE_INCLUDEDIR}/Host.cpp
    ${MM_MMCORE_INCLUDEDIR}/LibraryInfo/LibraryPathsWindows.cpp
    ${MM_MMCORE_INCLUDEDIR}/LoadableModules/LoadedDeviceAdapter.cpp
    ${MM_MMCORE_INCLUDEDIR}/LoadableModules/LoadedModule.cpp
    ${MM_MMCORE_INCLUDEDIR}/LoadableModules/LoadedModuleImpl.cpp
    ${MM_MMCORE_INCLUDEDIR}/LoadableModules/LoadedModuleImplWindows.cpp
    ${MM_MMCORE_INCLUDEDIR}/Logging/Metadata.cpp
    ${MM_MMCORE_INCLUDEDIR}/LogManager.cpp
    ${MM_MMCORE_INCLUDEDIR}/MMCore.cpp
    ${MM_MMCORE_INCLUDEDIR}/PluginManager.cpp
    ${MM_MMCORE_INCLUDEDIR}/Semaphore.cpp
    ${MM_MMCORE_INCLUDEDIR}/Task.cpp
    ${MM_MMCORE_INCLUDEDIR}/TaskSet.cpp
    ${MM_MMCORE_INCLUDEDIR}/TaskSet_CopyMemory.cpp
    ${MM_MMCORE_INCLUDEDIR}/ThreadPool.cpp

    ${MM_MMCORE_INCLUDEDIR}/CircularBuffer.h
    ${MM_MMCORE_INCLUDEDIR}/ConfigGroup.h
    ${MM_MMCORE_INCLUDEDIR}/Configuration.h
    ${MM_MMCORE_INCLUDEDIR}/CoreCallback.h
    ${MM_MMCORE_INCLUDEDIR}/CoreProperty.h
    ${MM_MMCORE_INCLUDEDIR}/CoreUtils.h
    ${MM_MMCORE_INCLUDEDIR}/DeviceManager.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/AutoFocusInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/CameraInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/DeviceInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/DeviceInstanceBase.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/DeviceInstances.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/GalvoInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/GenericInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/HubInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/ImageProcessorInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/MagnifierInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/SerialInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/ShutterInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/SignalIOInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/SLMInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/StageInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/StateInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Devices/XYStageInstance.h
    ${MM_MMCORE_INCLUDEDIR}/Error.h
    ${MM_MMCORE_INCLUDEDIR}/FrameBuffer.h
    ${MM_MMCORE_INCLUDEDIR}/Host.h
    ${MM_MMCORE_INCLUDEDIR}/LibraryInfo/LibraryPaths.h
    ${MM_MMCORE_INCLUDEDIR}/LoadableModules/LoadedDeviceAdapter.h
    ${MM_MMCORE_INCLUDEDIR}/LoadableModules/LoadedModule.h
    ${MM_MMCORE_INCLUDEDIR}/LoadableModules/LoadedModuleImpl.h
    ${MM_MMCORE_INCLUDEDIR}/LoadableModules/LoadedModuleImplWindows.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/GenericEntryFilter.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/GenericLinePacket.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/GenericLogger.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/GenericLoggingCore.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/GenericMetadata.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/GenericPacketArray.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/GenericPacketQueue.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/GenericSink.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/GenericStreamSink.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/Logger.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/Logging.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/Metadata.h
    ${MM_MMCORE_INCLUDEDIR}/Logging/MetadataFormatter.h
    ${MM_MMCORE_INCLUDEDIR}/LogManager.h
    ${MM_MMCORE_INCLUDEDIR}/MMCore.h
    ${MM_MMCORE_INCLUDEDIR}/MMEventCallback.h
    ${MM_MMCORE_INCLUDEDIR}/PluginManager.h
    ${MM_MMCORE_INCLUDEDIR}/Semaphore.h
    ${MM_MMCORE_INCLUDEDIR}/Task.h
    ${MM_MMCORE_INCLUDEDIR}/TaskSet.h
    ${MM_MMCORE_INCLUDEDIR}/TaskSet_CopyMemory.h
    ${MM_MMCORE_INCLUDEDIR}/ThreadPool.h
)

set(MMDEVICE_SRC 
    ${MM_MMDEVICE_INCLUDEDIR}/Debayer.cpp
    ${MM_MMDEVICE_INCLUDEDIR}/DeviceUtils.cpp
    ${MM_MMDEVICE_INCLUDEDIR}/ImgBuffer.cpp
    ${MM_MMDEVICE_INCLUDEDIR}/MMDevice.cpp
    ${MM_MMDEVICE_INCLUDEDIR}/ModuleInterface.cpp
    ${MM_MMDEVICE_INCLUDEDIR}/Property.cpp
)

add_library(MMCore ${MMCORE_SRC})
target_include_directories(MMCore PRIVATE ${MM_MMCORE_INCLUDEDIR} ${MM_MMDEVICE_INCLUDEDIR})
target_compile_definitions(MMCore PUBLIC ${MMCORE_DEFINITIONS})
target_compile_features(MMCore PUBLIC cxx_std_14)
target_link_libraries(MMCore PRIVATE Boost::boost Boost::system)

#add_library(MMDevice-Shared ${MMDEVICE_SRC})
#target_include_directories(MMDevice-Shared PRIVATE ${MM_MMDEVICE_INCLUDEDIR})
#target_compile_definitions(MMDevice-Shared PUBLIC ${MMDEVICE_DEFINITIONS})

#set(TARGET_NAME "mmgr_dal_${PROJECT_NAME}")
#add_library(${TARGET_NAME} SHARED TeensyDAC.cpp TeensyDAC.h)
#target_include_directories(${TARGET_NAME} PRIVATE ${MM_MMDEVICE_INCLUDEDIR} ../lib/ArduinoJson/src ../lib/Ardulingua/src)
#target_link_libraries(${TARGET_NAME} MMDevice-Shared)
#target_compile_definitions(${TARGET_NAME} PUBLIC ${MMDEVICE_DEFINITIONS})

#install(TARGETS ${TARGET_NAME} DESTINATION ${MM_INSTALLDIR})
