# Install script for directory: /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-cli.1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui.1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluMount-cli.1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluMount-gui.1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluSafe-cli.1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/zuluCrypt" TYPE FILE FILES "/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/docs/zuluCrypt.pdf")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/zuluCrypt/translations/zuluCrypt-gui" TYPE FILE FILES "/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/translations/zuluCrypt/de_DE.qm")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/zuluCrypt/translations/zuluCrypt-gui" TYPE FILE FILES "/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/translations/zuluCrypt/fr_FR.qm")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/zuluCrypt/translations/zuluMount-gui" TYPE FILE FILES "/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/translations/zuluMount/fr_FR.qm")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/external_libraries/tc-play/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluCrypt-cli/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluSafe/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluCrypt-gui/lxqt_wallet/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluCrypt-gui/md5/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluCrypt-gui/sharedObjects/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluCrypt-gui/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/plugins/cmake_install.cmake")
  include("/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluMount-gui/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
