# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/duan/xface-new

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/duan/xface-new

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/duan/xface-new/CMakeFiles /home/duan/xface-new/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/duan/xface-new/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named XSmilAgent

# Build rule for target.
XSmilAgent: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 XSmilAgent
.PHONY : XSmilAgent

# fast build rule for target.
XSmilAgent/fast:
	$(MAKE) -f XSmilAgent/CMakeFiles/XSmilAgent.dir/build.make XSmilAgent/CMakeFiles/XSmilAgent.dir/build
.PHONY : XSmilAgent/fast

#=============================================================================
# Target rules for targets named XFace

# Build rule for target.
XFace: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 XFace
.PHONY : XFace

# fast build rule for target.
XFace/fast:
	$(MAKE) -f XFace/CMakeFiles/XFace.dir/build.make XFace/CMakeFiles/XFace.dir/build
.PHONY : XFace/fast

#=============================================================================
# Target rules for targets named wxXFacePlayer

# Build rule for target.
wxXFacePlayer: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 wxXFacePlayer
.PHONY : wxXFacePlayer

# fast build rule for target.
wxXFacePlayer/fast:
	$(MAKE) -f wxFacePlayer/CMakeFiles/wxXFacePlayer.dir/build.make wxFacePlayer/CMakeFiles/wxXFacePlayer.dir/build
.PHONY : wxXFacePlayer/fast

#=============================================================================
# Target rules for targets named wxfaceplayer

# Build rule for target.
wxfaceplayer: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 wxfaceplayer
.PHONY : wxfaceplayer

# fast build rule for target.
wxfaceplayer/fast:
	$(MAKE) -f wxFacePlayer/CMakeFiles/wxfaceplayer.dir/build.make wxFacePlayer/CMakeFiles/wxfaceplayer.dir/build
.PHONY : wxfaceplayer/fast

#=============================================================================
# Target rules for targets named QFace

# Build rule for target.
QFace: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 QFace
.PHONY : QFace

# fast build rule for target.
QFace/fast:
	$(MAKE) -f QFace/CMakeFiles/QFace.dir/build.make QFace/CMakeFiles/QFace.dir/build
.PHONY : QFace/fast

#=============================================================================
# Target rules for targets named wxFaceEd

# Build rule for target.
wxFaceEd: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 wxFaceEd
.PHONY : wxFaceEd

# fast build rule for target.
wxFaceEd/fast:
	$(MAKE) -f wxFaceEd/CMakeFiles/wxFaceEd.dir/build.make wxFaceEd/CMakeFiles/wxFaceEd.dir/build
.PHONY : wxFaceEd/fast

#=============================================================================
# Target rules for targets named wxfaceclient

# Build rule for target.
wxfaceclient: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 wxfaceclient
.PHONY : wxfaceclient

# fast build rule for target.
wxfaceclient/fast:
	$(MAKE) -f wxFaceClient/CMakeFiles/wxfaceclient.dir/build.make wxFaceClient/CMakeFiles/wxfaceclient.dir/build
.PHONY : wxfaceclient/fast

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... XSmilAgent"
	@echo "... XFace"
	@echo "... wxXFacePlayer"
	@echo "... wxfaceplayer"
	@echo "... QFace"
	@echo "... wxFaceEd"
	@echo "... wxfaceclient"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

