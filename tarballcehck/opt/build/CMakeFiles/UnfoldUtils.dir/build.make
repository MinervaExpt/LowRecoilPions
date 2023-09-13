# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake

# The command to remove a file.
RM = /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/MAT-MINERvA/bootstrap

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build

# Utility rule file for UnfoldUtils.

# Include the progress variables for this target.
include CMakeFiles/UnfoldUtils.dir/progress.make

CMakeFiles/UnfoldUtils: CMakeFiles/UnfoldUtils-complete


CMakeFiles/UnfoldUtils-complete: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-install
CMakeFiles/UnfoldUtils-complete: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-mkdir
CMakeFiles/UnfoldUtils-complete: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-download
CMakeFiles/UnfoldUtils-complete: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-patch
CMakeFiles/UnfoldUtils-complete: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-configure
CMakeFiles/UnfoldUtils-complete: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-build
CMakeFiles/UnfoldUtils-complete: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'UnfoldUtils'"
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E touch /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles/UnfoldUtils-complete
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E touch /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-done

UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-install: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing install step for 'UnfoldUtils'"
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build && $(MAKE) install
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build && /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E touch /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-install

UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'UnfoldUtils'"
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/MAT-MINERvA/bootstrap/../../UnfoldUtils
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/tmp
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-stamp
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E touch /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-mkdir

UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-download: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'UnfoldUtils'"
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src && /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E echo_append
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src && /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E touch /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-download

UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-patch: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No patch step for 'UnfoldUtils'"
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E echo_append
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E touch /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-patch

UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-configure: MAT-prefix/src/MAT-stamp/MAT-done
UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-configure: MAT-MINERvA-prefix/src/MAT-MINERvA-stamp/MAT-MINERvA-done
UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-configure: UnfoldUtils-prefix/tmp/UnfoldUtils-cfgcmd.txt
UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-configure: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-skip-update
UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-configure: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Performing configure step for 'UnfoldUtils'"
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build && /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -DCMAKE_INSTALL_PREFIX:PATH=/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt -DCMAKE_BUILD_TYPE:STRING=Release "-GUnix Makefiles" /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/MAT-MINERvA/bootstrap/../../UnfoldUtils
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build && /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E touch /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-configure

UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-build: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Performing build step for 'UnfoldUtils'"
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build && $(MAKE)
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-build && /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E touch /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-build

UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-skip-update: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Skipping update step for 'UnfoldUtils'"
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils && /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E echo_append

UnfoldUtils: CMakeFiles/UnfoldUtils
UnfoldUtils: CMakeFiles/UnfoldUtils-complete
UnfoldUtils: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-install
UnfoldUtils: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-mkdir
UnfoldUtils: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-download
UnfoldUtils: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-patch
UnfoldUtils: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-configure
UnfoldUtils: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-build
UnfoldUtils: UnfoldUtils-prefix/src/UnfoldUtils-stamp/UnfoldUtils-skip-update
UnfoldUtils: CMakeFiles/UnfoldUtils.dir/build.make

.PHONY : UnfoldUtils

# Rule to build all files generated by this target.
CMakeFiles/UnfoldUtils.dir/build: UnfoldUtils

.PHONY : CMakeFiles/UnfoldUtils.dir/build

CMakeFiles/UnfoldUtils.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/UnfoldUtils.dir/cmake_clean.cmake
.PHONY : CMakeFiles/UnfoldUtils.dir/clean

CMakeFiles/UnfoldUtils.dir/depend:
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/MAT-MINERvA/bootstrap /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/MAT-MINERvA/bootstrap /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles/UnfoldUtils.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/UnfoldUtils.dir/depend
