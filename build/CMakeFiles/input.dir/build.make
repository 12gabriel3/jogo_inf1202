# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Gabriel\Documents\Jogo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Gabriel\Documents\Jogo\build

# Include any dependencies generated for this target.
include CMakeFiles/input.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/input.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/input.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/input.dir/flags.make

CMakeFiles/input.dir/src/input.c.obj: CMakeFiles/input.dir/flags.make
CMakeFiles/input.dir/src/input.c.obj: CMakeFiles/input.dir/includes_C.rsp
CMakeFiles/input.dir/src/input.c.obj: ../src/input.c
CMakeFiles/input.dir/src/input.c.obj: CMakeFiles/input.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Gabriel\Documents\Jogo\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/input.dir/src/input.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/input.dir/src/input.c.obj -MF CMakeFiles\input.dir\src\input.c.obj.d -o CMakeFiles\input.dir\src\input.c.obj -c C:\Users\Gabriel\Documents\Jogo\src\input.c

CMakeFiles/input.dir/src/input.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/input.dir/src/input.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Gabriel\Documents\Jogo\src\input.c > CMakeFiles\input.dir\src\input.c.i

CMakeFiles/input.dir/src/input.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/input.dir/src/input.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Gabriel\Documents\Jogo\src\input.c -o CMakeFiles\input.dir\src\input.c.s

# Object files for target input
input_OBJECTS = \
"CMakeFiles/input.dir/src/input.c.obj"

# External object files for target input
input_EXTERNAL_OBJECTS =

libinput.a: CMakeFiles/input.dir/src/input.c.obj
libinput.a: CMakeFiles/input.dir/build.make
libinput.a: CMakeFiles/input.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Gabriel\Documents\Jogo\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libinput.a"
	$(CMAKE_COMMAND) -P CMakeFiles\input.dir\cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\input.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/input.dir/build: libinput.a
.PHONY : CMakeFiles/input.dir/build

CMakeFiles/input.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\input.dir\cmake_clean.cmake
.PHONY : CMakeFiles/input.dir/clean

CMakeFiles/input.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Gabriel\Documents\Jogo C:\Users\Gabriel\Documents\Jogo C:\Users\Gabriel\Documents\Jogo\build C:\Users\Gabriel\Documents\Jogo\build C:\Users\Gabriel\Documents\Jogo\build\CMakeFiles\input.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/input.dir/depend

