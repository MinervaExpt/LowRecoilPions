# Install script for directory: /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/GENIEXSecExtract/src

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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGENIEXSecExtract.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGENIEXSecExtract.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGENIEXSecExtract.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/src/libGENIEXSecExtract.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGENIEXSecExtract.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGENIEXSecExtract.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGENIEXSecExtract.so"
         OLD_RPATH "/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/root/v6_10_04d/Linux64bit+3.10-2.17-e14-prof/lib:/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libGENIEXSecExtract.so")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/GENIEXSecExtract/GENIEXSecExtractTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/GENIEXSecExtract/GENIEXSecExtractTargets.cmake"
         "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/src/CMakeFiles/Export/lib/cmake/GENIEXSecExtract/GENIEXSecExtractTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/GENIEXSecExtract/GENIEXSecExtractTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/GENIEXSecExtract/GENIEXSecExtractTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/GENIEXSecExtract" TYPE FILE FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/src/CMakeFiles/Export/lib/cmake/GENIEXSecExtract/GENIEXSecExtractTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/GENIEXSecExtract" TYPE FILE FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build_GENIEXSecExtract/src/CMakeFiles/Export/lib/cmake/GENIEXSecExtract/GENIEXSecExtractTargets-release.cmake")
  endif()
endif()

