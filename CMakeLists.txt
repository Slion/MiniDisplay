cmake_minimum_required(VERSION 2.8)

project(MiniDisplay)
# Allow the developer to select if Dynamic or Static libraries are built
option (MD_SHARED_LIB "Build Shared Libraries" ON)
# Set the LIB_TYPE variable to STATIC
set (LIB_TYPE STATIC)
if (MD_SHARED_LIB)
  # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
  set (LIB_TYPE SHARED)
endif (MD_SHARED_LIB)

set(SOURCES		BitArray.cpp
				FutabaVfd.cpp
				FutabaGP1212.cpp
				FutabaGP1212A01.cpp
				FutabaGP1212A02.cpp
				HidDevice.cpp
				MiniDisplay.cpp
				../../GitHub/hidapi/windows/hid.c)

source_group(Souces FILES ${SOURCES})

set(INCLUDES	BitArray.h
				FutabaVfd.h
				FutabaGP1212.h
				FutabaGP1212A01.h
				FutabaGP1212A02.h
				HidDevice.h
				HidReport.h
				MiniDisplay.h
				Display.h
				)

source_group(Includes FILES ${INCLUDES})

include_directories(../../GitHub/hidapi/hidapi)

# Create a target for the library
add_library(MiniDisplay ${LIB_TYPE} ${SOURCES} ${INCLUDES})

target_link_libraries(MiniDisplay setupapi)