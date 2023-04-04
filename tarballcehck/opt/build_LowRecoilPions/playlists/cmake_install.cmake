# Install script for directory: /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/playlists

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/etc/playlists" TYPE FILE FILES
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/playlists/CCQENu_minervame1A_DATA_Inextinguishable_merged.txt"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/playlists/CCQENu_minervame1A_MC_Inextinguishable_merged.txt"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/playlists/MAD_minervame1A_MC_andrewsGPVM.txt"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/playlists/MAD_minervame1A_DATA_andrewsGPVM.txt"
    )
endif()

