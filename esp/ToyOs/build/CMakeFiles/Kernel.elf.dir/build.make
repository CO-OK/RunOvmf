# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/v/program/EFI/runOvmf/esp/ToyOs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/v/program/EFI/runOvmf/esp/ToyOs/build

# Include any dependencies generated for this target.
include CMakeFiles/Kernel.elf.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Kernel.elf.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Kernel.elf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Kernel.elf.dir/flags.make

CMakeFiles/Kernel.elf.dir/Kernel.c.o: CMakeFiles/Kernel.elf.dir/flags.make
CMakeFiles/Kernel.elf.dir/Kernel.c.o: ../Kernel.c
CMakeFiles/Kernel.elf.dir/Kernel.c.o: CMakeFiles/Kernel.elf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/v/program/EFI/runOvmf/esp/ToyOs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Kernel.elf.dir/Kernel.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Kernel.elf.dir/Kernel.c.o -MF CMakeFiles/Kernel.elf.dir/Kernel.c.o.d -o CMakeFiles/Kernel.elf.dir/Kernel.c.o -c /home/v/program/EFI/runOvmf/esp/ToyOs/Kernel.c

CMakeFiles/Kernel.elf.dir/Kernel.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Kernel.elf.dir/Kernel.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/v/program/EFI/runOvmf/esp/ToyOs/Kernel.c > CMakeFiles/Kernel.elf.dir/Kernel.c.i

CMakeFiles/Kernel.elf.dir/Kernel.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Kernel.elf.dir/Kernel.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/v/program/EFI/runOvmf/esp/ToyOs/Kernel.c -o CMakeFiles/Kernel.elf.dir/Kernel.c.s

CMakeFiles/Kernel.elf.dir/Video.c.o: CMakeFiles/Kernel.elf.dir/flags.make
CMakeFiles/Kernel.elf.dir/Video.c.o: ../Video.c
CMakeFiles/Kernel.elf.dir/Video.c.o: CMakeFiles/Kernel.elf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/v/program/EFI/runOvmf/esp/ToyOs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Kernel.elf.dir/Video.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Kernel.elf.dir/Video.c.o -MF CMakeFiles/Kernel.elf.dir/Video.c.o.d -o CMakeFiles/Kernel.elf.dir/Video.c.o -c /home/v/program/EFI/runOvmf/esp/ToyOs/Video.c

CMakeFiles/Kernel.elf.dir/Video.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Kernel.elf.dir/Video.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/v/program/EFI/runOvmf/esp/ToyOs/Video.c > CMakeFiles/Kernel.elf.dir/Video.c.i

CMakeFiles/Kernel.elf.dir/Video.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Kernel.elf.dir/Video.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/v/program/EFI/runOvmf/esp/ToyOs/Video.c -o CMakeFiles/Kernel.elf.dir/Video.c.s

# Object files for target Kernel.elf
Kernel_elf_OBJECTS = \
"CMakeFiles/Kernel.elf.dir/Kernel.c.o" \
"CMakeFiles/Kernel.elf.dir/Video.c.o"

# External object files for target Kernel.elf
Kernel_elf_EXTERNAL_OBJECTS =

Kernel.elf: CMakeFiles/Kernel.elf.dir/Kernel.c.o
Kernel.elf: CMakeFiles/Kernel.elf.dir/Video.c.o
Kernel.elf: CMakeFiles/Kernel.elf.dir/build.make
Kernel.elf: CMakeFiles/Kernel.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/v/program/EFI/runOvmf/esp/ToyOs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable Kernel.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Kernel.elf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Kernel.elf.dir/build: Kernel.elf
.PHONY : CMakeFiles/Kernel.elf.dir/build

CMakeFiles/Kernel.elf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Kernel.elf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Kernel.elf.dir/clean

CMakeFiles/Kernel.elf.dir/depend:
	cd /home/v/program/EFI/runOvmf/esp/ToyOs/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/v/program/EFI/runOvmf/esp/ToyOs /home/v/program/EFI/runOvmf/esp/ToyOs /home/v/program/EFI/runOvmf/esp/ToyOs/build /home/v/program/EFI/runOvmf/esp/ToyOs/build /home/v/program/EFI/runOvmf/esp/ToyOs/build/CMakeFiles/Kernel.elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Kernel.elf.dir/depend
