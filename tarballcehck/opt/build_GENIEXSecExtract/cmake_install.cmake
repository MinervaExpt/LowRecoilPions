# Install script for directory: /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/GENIEXSecExtract

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/setup_GENIEXSecExtract.sh")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/GENIEXSecExtract" TYPE FILE FILES
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/GENIEXSecExtractConfig.cmake"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/GENIEXSecExtractConfigVersion.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/GENIEXSecExtract/cmake_install.cmake")
  include("/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/src/cmake_install.cmake")
  include("/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/apps/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
