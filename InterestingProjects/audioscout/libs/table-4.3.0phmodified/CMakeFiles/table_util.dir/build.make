# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/david/projects/audioscout

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/david/projects/audioscout

# Include any dependencies generated for this target.
include libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/depend.make

# Include the progress variables for this target.
include libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/progress.make

# Include the compile flags for this target's objects.
include libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/flags.make

libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o: libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/flags.make
libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o: libs/table-4.3.0phmodified/table_util.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/david/projects/audioscout/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o"
	cd /home/david/projects/audioscout/libs/table-4.3.0phmodified && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/table_util.dir/table_util.c.o   -c /home/david/projects/audioscout/libs/table-4.3.0phmodified/table_util.c

libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/table_util.dir/table_util.c.i"
	cd /home/david/projects/audioscout/libs/table-4.3.0phmodified && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/david/projects/audioscout/libs/table-4.3.0phmodified/table_util.c > CMakeFiles/table_util.dir/table_util.c.i

libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/table_util.dir/table_util.c.s"
	cd /home/david/projects/audioscout/libs/table-4.3.0phmodified && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/david/projects/audioscout/libs/table-4.3.0phmodified/table_util.c -o CMakeFiles/table_util.dir/table_util.c.s

libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o.requires:
.PHONY : libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o.requires

libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o.provides: libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o.requires
	$(MAKE) -f libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/build.make libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o.provides.build
.PHONY : libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o.provides

libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o.provides.build: libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o
.PHONY : libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o.provides.build

# Object files for target table_util
table_util_OBJECTS = \
"CMakeFiles/table_util.dir/table_util.c.o"

# External object files for target table_util
table_util_EXTERNAL_OBJECTS =

libs/table-4.3.0phmodified/table_util: libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o
libs/table-4.3.0phmodified/table_util: libs/table-4.3.0phmodified/libtable.a
libs/table-4.3.0phmodified/table_util: libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/build.make
libs/table-4.3.0phmodified/table_util: libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable table_util"
	cd /home/david/projects/audioscout/libs/table-4.3.0phmodified && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/table_util.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/build: libs/table-4.3.0phmodified/table_util
.PHONY : libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/build

libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/requires: libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/table_util.c.o.requires
.PHONY : libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/requires

libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/clean:
	cd /home/david/projects/audioscout/libs/table-4.3.0phmodified && $(CMAKE_COMMAND) -P CMakeFiles/table_util.dir/cmake_clean.cmake
.PHONY : libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/clean

libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/depend:
	cd /home/david/projects/audioscout && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/david/projects/audioscout /home/david/projects/audioscout/libs/table-4.3.0phmodified /home/david/projects/audioscout /home/david/projects/audioscout/libs/table-4.3.0phmodified /home/david/projects/audioscout/libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/table-4.3.0phmodified/CMakeFiles/table_util.dir/depend

