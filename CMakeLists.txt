cmake_minimum_required(VERSION 3.8.0)
project(TeensyDAC2 VERSION 0.1.0)

set(CMAKE_VERBOSE_MAKEFILE ON)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)

set( Boost_THREADAPI win32)

#find_package(Boost 1.77 REQUIRED COMPONENTS thread asio)
find_package(Boost 1.77 REQUIRED COMPONENTS thread)
find_package(catch2 2 CONFIG REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

if(DEFINED ENV{MM_SRCROOT})
    set(MM_SRCROOT ENV{MM_SRCROOT})
else()
    set(MM_SRCROOT "${CMAKE_SOURCE_DIR}/../../mmCoreAndDevices")
endif()

if (DEFINED ENV{MM_INSTALLDIR})
    set(MM_INSTALLDIR ENV{MM_INSTALLDIR})
else()
    set(MM_INSTALLDIR "C:/Apps/Micro-Manager2")
endif()

if (DEFINED ENV{MM_3RDPARTYPUBLIC})
    set(MM_3RDPARTYPUBLIC ENV{MM_3RDPARTYPUBLIC})
else()
    set(MM_3RDPARTYPUBLIC "${CMAKE_SOURCE_DIR}/../../../3rdpartyPublic")
endif()

set(MM_3RDPARTYPUBLIC "${CMAKE_SOURCE_DIR}/../../../3rdpartyPublic")

#get_filename_component(MM_SRCROOT_ABSOLUTE_PATH "${MM_SRCROOT}" ABSOLUTE)
#get_filename_component(MM_INSTALLDIR_ABSOLUTE_PATH "${MM_INSTALLDIR}" ABSOLUTE)
#get_filename_component(MM_3RDPARTYPUBLIC_ABSOLUTE_PATH "${MM_3RDPARTYPUBLIC}" ABSOLUTE)
#MESSAGE( STATUS "MM_SRCROOT:               " ${MM_SRCROOT_ABSOLUTE_PATH} )
#MESSAGE( STATUS "MM_INSTALLDIR:            " ${MM_INSTALLDIR_ABSOLUTE_PATH} )
#MESSAGE( STATUS "MM_3RDPARTYPUBLIC:        " ${MM_3RDPARTYPUBLIC_ABSOLUTE_PATH} )

include(MMCore.cmake)
include(MMDevice-Shared.cmake)
include(SerialManager.cmake)

add_subdirectory(firmware)
add_subdirectory(src)
add_subdirectory(test)
