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

# Utility rule file for MAT-update.

# Include the progress variables for this target.
include CMakeFiles/MAT-update.dir/progress.make

CMakeFiles/MAT-update: MAT-prefix/src/MAT-stamp/MAT-update


MAT-prefix/src/MAT-stamp/MAT-update: MAT-prefix/src/MAT-stamp/MAT-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Performing update step for 'MAT'"
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/MAT && /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -P /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/MAT-prefix/tmp/MAT-gitupdate.cmake

MAT-prefix/src/MAT-stamp/MAT-download: MAT-prefix/src/MAT-stamp/MAT-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No download step for 'MAT'"
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/MAT-prefix/src && /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E echo_append
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/MAT-prefix/src && /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E touch /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/MAT-prefix/src/MAT-stamp/MAT-download

MAT-prefix/src/MAT-stamp/MAT-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'MAT'"
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/MAT-MINERvA/bootstrap/../../MAT
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/MAT-prefix/src/MAT-build
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/MAT-prefix
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/MAT-prefix/tmp
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/MAT-prefix/src/MAT-stamp
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E make_directory /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/MAT-prefix/src
	/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/cmake/v3_7_1/Linux64bit+3.10-2.17/bin/cmake -E touch /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/MAT-prefix/src/MAT-stamp/MAT-mkdir

MAT-update: CMakeFiles/MAT-update
MAT-update: MAT-prefix/src/MAT-stamp/MAT-update
MAT-update: MAT-prefix/src/MAT-stamp/MAT-download
MAT-update: MAT-prefix/src/MAT-stamp/MAT-mkdir
MAT-update: CMakeFiles/MAT-update.dir/build.make

.PHONY : MAT-update

# Rule to build all files generated by this target.
CMakeFiles/MAT-update.dir/build: MAT-update

.PHONY : CMakeFiles/MAT-update.dir/build

CMakeFiles/MAT-update.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MAT-update.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MAT-update.dir/clean

CMakeFiles/MAT-update.dir/depend:
	cd /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/MAT-MINERvA/bootstrap /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/MAT-MINERvA/bootstrap /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build /minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/opt/build/CMakeFiles/MAT-update.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MAT-update.dir/depend

