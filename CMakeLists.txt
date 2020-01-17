cmake_minimum_required (VERSION 3.6)

if( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin" )
    set(SOS_TOOLCHAIN_CMAKE_PATH /Applications/StratifyLabs-SDK/Tools/gcc/arm-none-eabi/cmake)
elseif( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows" )
    set(SOS_TOOLCHAIN_CMAKE_PATH C:/StratifyLabs-SDK/Tools/gcc/arm-none-eabi/cmake)
else()
  message(FATAL_ERROR "Cross compiling is not yet supportd on: " ${CMAKE_HOST_SYSTEM_NAME})
endif()

include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-variables.cmake)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-sdk.cmake)

# This will set the default RAM used by the application
set(SOS_RAM_SIZE 8192)

if( ${SOS_BUILD_CONFIG} STREQUAL arm )
	set(CMAKE_TOOLCHAIN_FILE ${SOS_TOOLCHAIN_CMAKE_PATH}/sos-app-toolchain.cmake)
elseif( ${SOS_BUILD_CONFIG} STREQUAL link )
	set(CMAKE_TOOLCHAIN_FILE ${SOS_TOOLCHAIN_CMAKE_PATH}/link-toolchain.cmake)
endif()

get_filename_component(SOS_NAME ${CMAKE_SOURCE_DIR} NAME)
project(${SOS_NAME} CXX C)

#Add sources to the project
sos_sdk_add_subdirectory(SOS_SOURCELIST src)
list(APPEND SOS_SOURCELIST ${SOURCES} README.md)

message(STATUS "Building App sources: " ${SOS_SOURCELIST})

set(SOS_ARCH link)
set(SOS_LIBRARIES toolkit sgfx jansson mbedtls)
if( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows" )
	set(SOS_LIBRARIES api ${SOS_LIBRARIES} ws2_32 wsock32)
endif()
set(SOS_CONFIG release)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-app.cmake)

install(
	PROGRAMS
	${CMAKE_SOURCE_DIR}/build_release_link/${SOS_NAME}_release${SOS_SDK_EXEC_SUFFIX}
	DESTINATION ${SOS_TOOLCHAIN_CMAKE_PATH}/../../bin
	RENAME ${SOS_NAME}${SOS_SDK_EXEC_SUFFIX}
	)



