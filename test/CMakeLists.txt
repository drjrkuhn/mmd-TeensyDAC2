cmake_minimum_required(VERSION 3.8.0)
project("TeensyDAC2-Test" VERSION 0.1.0)


include(MMCore.cmake)

set(UNITTEST_DEFINITIONS _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS WIN32 _DEBUG _LIB _WINDOWS _USRDLL MODULE_EXPORTS)

add_executable(${PROJECT_NAME} DeviceTestMain.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_include_directories(${PROJECT_NAME} PRIVATE ${MM_MMDEVICE_INCLUDEDIR} ${MM_MMCORE_INCLUDEDIR} ../lib/Ardulingua/src)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${UNITTEST_DEFINITIONS})
target_link_libraries(${PROJECT_NAME} MMCore MMDevice-Shared Boost::boost Boost::thread)
