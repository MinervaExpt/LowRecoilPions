# Install script for directory: /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build/UnfoldUtilsDict.rootmap")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build/UnfoldUtils_rdict.pcm")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUnfoldUtils.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUnfoldUtils.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUnfoldUtils.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build/libUnfoldUtils.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUnfoldUtils.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUnfoldUtils.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUnfoldUtils.so"
         OLD_RPATH "/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/root/v6_10_04d/Linux64bit+3.10-2.17-e14-prof/lib:/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUnfoldUtils.so")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  EXECUTE_PROCESS( COMMAND /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E create_symlink libUnfoldUtils.so /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/lib/libUnfoldUtilsDict.so )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/MinervaUnfold" TYPE FILE FILES
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/MinervaUnfold/MnvResponse.h"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/MinervaUnfold/MnvUnfold.h"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/MinervaUnfold/UnfoldCovmxAsErrorBand.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/RooUnfold" TYPE FILE FILES
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/RooUnfold/RooUnfold.h"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/RooUnfold/RooUnfoldBayes.h"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/RooUnfold/RooUnfoldBinByBin.h"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/RooUnfold/RooUnfoldErrors.h"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/RooUnfold/RooUnfoldInvert.h"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/RooUnfold/RooUnfoldParms.h"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/RooUnfold/RooUnfoldResponse.h"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/RooUnfold/RooUnfoldSvd.h"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/RooUnfold/RooUnfoldTUnfold.h"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/RooUnfold/TSVDUnfold_local.h"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/RooUnfold/matrix_mult_mt.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/UnfoldUtils/UnfoldUtilsTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/UnfoldUtils/UnfoldUtilsTargets.cmake"
         "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build/CMakeFiles/Export/lib/cmake/UnfoldUtils/UnfoldUtilsTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/UnfoldUtils/UnfoldUtilsTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/UnfoldUtils/UnfoldUtilsTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/UnfoldUtils" TYPE FILE FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build/CMakeFiles/Export/lib/cmake/UnfoldUtils/UnfoldUtilsTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/UnfoldUtils" TYPE FILE FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build/CMakeFiles/Export/lib/cmake/UnfoldUtils/UnfoldUtilsTargets-release.cmake")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/TransWarpExtraction" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/TransWarpExtraction")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/TransWarpExtraction"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build/TransWarpExtraction")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/TransWarpExtraction" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/TransWarpExtraction")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/TransWarpExtraction"
         OLD_RPATH "/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/root/v6_10_04d/Linux64bit+3.10-2.17-e14-prof/lib:/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build:/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/TransWarpExtraction")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build/setup_UnfoldUtils.sh")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/UnfoldUtils" TYPE FILE FILES
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build/UnfoldUtilsConfig.cmake"
    "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build/UnfoldUtilsConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
