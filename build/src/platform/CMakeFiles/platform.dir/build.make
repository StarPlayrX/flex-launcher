# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/toddbruss/Documents/GitHub/flex-launcher

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/toddbruss/Documents/GitHub/flex-launcher/build

# Include any dependencies generated for this target.
include src/platform/CMakeFiles/platform.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/platform/CMakeFiles/platform.dir/compiler_depend.make

# Include the progress variables for this target.
include src/platform/CMakeFiles/platform.dir/progress.make

# Include the compile flags for this target's objects.
include src/platform/CMakeFiles/platform.dir/flags.make

src/platform/CMakeFiles/platform.dir/unix.c.o: src/platform/CMakeFiles/platform.dir/flags.make
src/platform/CMakeFiles/platform.dir/unix.c.o: /Users/toddbruss/Documents/GitHub/flex-launcher/src/platform/unix.c
src/platform/CMakeFiles/platform.dir/unix.c.o: src/platform/CMakeFiles/platform.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/toddbruss/Documents/GitHub/flex-launcher/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/platform/CMakeFiles/platform.dir/unix.c.o"
	cd /Users/toddbruss/Documents/GitHub/flex-launcher/build/src/platform && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/platform/CMakeFiles/platform.dir/unix.c.o -MF CMakeFiles/platform.dir/unix.c.o.d -o CMakeFiles/platform.dir/unix.c.o -c /Users/toddbruss/Documents/GitHub/flex-launcher/src/platform/unix.c

src/platform/CMakeFiles/platform.dir/unix.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/platform.dir/unix.c.i"
	cd /Users/toddbruss/Documents/GitHub/flex-launcher/build/src/platform && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/toddbruss/Documents/GitHub/flex-launcher/src/platform/unix.c > CMakeFiles/platform.dir/unix.c.i

src/platform/CMakeFiles/platform.dir/unix.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/platform.dir/unix.c.s"
	cd /Users/toddbruss/Documents/GitHub/flex-launcher/build/src/platform && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/toddbruss/Documents/GitHub/flex-launcher/src/platform/unix.c -o CMakeFiles/platform.dir/unix.c.s

# Object files for target platform
platform_OBJECTS = \
"CMakeFiles/platform.dir/unix.c.o"

# External object files for target platform
platform_EXTERNAL_OBJECTS =

src/platform/libplatform.a: src/platform/CMakeFiles/platform.dir/unix.c.o
src/platform/libplatform.a: src/platform/CMakeFiles/platform.dir/build.make
src/platform/libplatform.a: src/platform/CMakeFiles/platform.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/toddbruss/Documents/GitHub/flex-launcher/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libplatform.a"
	cd /Users/toddbruss/Documents/GitHub/flex-launcher/build/src/platform && $(CMAKE_COMMAND) -P CMakeFiles/platform.dir/cmake_clean_target.cmake
	cd /Users/toddbruss/Documents/GitHub/flex-launcher/build/src/platform && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/platform.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/platform/CMakeFiles/platform.dir/build: src/platform/libplatform.a
.PHONY : src/platform/CMakeFiles/platform.dir/build

src/platform/CMakeFiles/platform.dir/clean:
	cd /Users/toddbruss/Documents/GitHub/flex-launcher/build/src/platform && $(CMAKE_COMMAND) -P CMakeFiles/platform.dir/cmake_clean.cmake
.PHONY : src/platform/CMakeFiles/platform.dir/clean

src/platform/CMakeFiles/platform.dir/depend:
	cd /Users/toddbruss/Documents/GitHub/flex-launcher/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/toddbruss/Documents/GitHub/flex-launcher /Users/toddbruss/Documents/GitHub/flex-launcher/src/platform /Users/toddbruss/Documents/GitHub/flex-launcher/build /Users/toddbruss/Documents/GitHub/flex-launcher/build/src/platform /Users/toddbruss/Documents/GitHub/flex-launcher/build/src/platform/CMakeFiles/platform.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/platform/CMakeFiles/platform.dir/depend

