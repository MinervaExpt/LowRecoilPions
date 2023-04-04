# Install script for directory: /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/GENIEXSecExtract/apps

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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runXSecLooper_CCQENuInclusive" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runXSecLooper_CCQENuInclusive")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runXSecLooper_CCQENuInclusive"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/apps/runXSecLooper_CCQENuInclusive")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runXSecLooper_CCQENuInclusive" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runXSecLooper_CCQENuInclusive")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runXSecLooper_CCQENuInclusive"
         OLD_RPATH "/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/root/v6_10_04d/Linux64bit+3.10-2.17-e14-prof/lib:/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/src:/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runXSecLooper_CCQENuInclusive")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForMultiNeutron" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForMultiNeutron")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForMultiNeutron"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/apps/runCCIncForMultiNeutron")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForMultiNeutron" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForMultiNeutron")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForMultiNeutron"
         OLD_RPATH "/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/root/v6_10_04d/Linux64bit+3.10-2.17-e14-prof/lib:/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/src:/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForMultiNeutron")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForNuEMEC" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForNuEMEC")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForNuEMEC"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/apps/runCCIncForNuEMEC")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForNuEMEC" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForNuEMEC")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForNuEMEC"
         OLD_RPATH "/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/root/v6_10_04d/Linux64bit+3.10-2.17-e14-prof/lib:/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/src:/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/runCCIncForNuEMEC")
    endif()
  endif()
endif()

