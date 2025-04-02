# Install script for directory: /home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/avr-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/MGIE/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/MDIO/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/MPORT/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/MEXTI/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/MTIMER/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/MUSART/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/MPWM/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/HLED/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/HSWITCH/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/H7SEGMENT/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/HSCHEDULER/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/HMOTOR/cmake_install.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/nemesis/Storage/Playground/Github_Repositories/ITI_EmbeddedAndroid/Controlling-a-Motor-Java-Project/MCU_MotorControl/APP/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
