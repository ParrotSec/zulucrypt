# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui

# Include any dependencies generated for this target.
include zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/depend.make

# Include the progress variables for this target.
include zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/progress.make

# Include the compile flags for this target's objects.
include zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/flags.make

zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o: zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/flags.make
zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o: ../plugins/test/zuluCrypt-testKey.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o"
	cd /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluCrypt-cli && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o   -c /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/plugins/test/zuluCrypt-testKey.c

zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.i"
	cd /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluCrypt-cli && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/plugins/test/zuluCrypt-testKey.c > CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.i

zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.s"
	cd /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluCrypt-cli && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/plugins/test/zuluCrypt-testKey.c -o CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.s

zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o.requires:

.PHONY : zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o.requires

zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o.provides: zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o.requires
	$(MAKE) -f zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/build.make zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o.provides.build
.PHONY : zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o.provides

zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o.provides.build: zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o


# Object files for target zuluCrypt-testKey
zuluCrypt__testKey_OBJECTS = \
"CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o"

# External object files for target zuluCrypt-testKey
zuluCrypt__testKey_EXTERNAL_OBJECTS =

zuluCrypt-cli/zuluCrypt-testKey: zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o
zuluCrypt-cli/zuluCrypt-testKey: zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/build.make
zuluCrypt-cli/zuluCrypt-testKey: zuluCrypt-cli/libzuluCryptPluginManager.so.1.0.0
zuluCrypt-cli/zuluCrypt-testKey: /usr/lib/x86_64-linux-gnu/libblkid.so
zuluCrypt-cli/zuluCrypt-testKey: zuluCrypt-cli/libSocket.a
zuluCrypt-cli/zuluCrypt-testKey: zuluCrypt-cli/libProcess.a
zuluCrypt-cli/zuluCrypt-testKey: zuluCrypt-cli/libString.a
zuluCrypt-cli/zuluCrypt-testKey: zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable zuluCrypt-testKey"
	cd /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluCrypt-cli && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zuluCrypt-testKey.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/build: zuluCrypt-cli/zuluCrypt-testKey

.PHONY : zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/build

zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/requires: zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/__/plugins/test/zuluCrypt-testKey.c.o.requires

.PHONY : zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/requires

zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/clean:
	cd /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluCrypt-cli && $(CMAKE_COMMAND) -P CMakeFiles/zuluCrypt-testKey.dir/cmake_clean.cmake
.PHONY : zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/clean

zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/depend:
	cd /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0 /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-cli /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluCrypt-cli /home/palinuro/Desktop/zulu/zulucrypt-5.0.0+parrot0/zuluCrypt-gui/zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : zuluCrypt-cli/CMakeFiles/zuluCrypt-testKey.dir/depend

