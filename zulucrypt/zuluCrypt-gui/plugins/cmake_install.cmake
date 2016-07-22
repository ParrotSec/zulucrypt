# Install script for directory: /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/plugins

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "None")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/plugins/keydialog-qt/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/plugins/gpg/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/plugins/keykeyfile/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/plugins/steghide/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/plugins/luks/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/plugins/tomb/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/plugins/hmac/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/plugins/generic_header/cmake_install.cmake")

endif()

