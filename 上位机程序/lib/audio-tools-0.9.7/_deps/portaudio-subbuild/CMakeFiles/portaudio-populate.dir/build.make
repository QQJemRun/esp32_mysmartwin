# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild

# Utility rule file for portaudio-populate.

# Include any custom commands dependencies for this target.
include CMakeFiles/portaudio-populate.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/portaudio-populate.dir/progress.make

CMakeFiles/portaudio-populate: CMakeFiles/portaudio-populate-complete

CMakeFiles/portaudio-populate-complete: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-install
CMakeFiles/portaudio-populate-complete: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-mkdir
CMakeFiles/portaudio-populate-complete: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-download
CMakeFiles/portaudio-populate-complete: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-patch
CMakeFiles/portaudio-populate-complete: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-configure
CMakeFiles/portaudio-populate-complete: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-build
CMakeFiles/portaudio-populate-complete: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-install
CMakeFiles/portaudio-populate-complete: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'portaudio-populate'"
	/usr/bin/cmake -E make_directory /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/CMakeFiles
	/usr/bin/cmake -E touch /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/CMakeFiles/portaudio-populate-complete
	/usr/bin/cmake -E touch /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-done

portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-build: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No build step for 'portaudio-populate'"
	cd /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-build && /usr/bin/cmake -E echo_append
	cd /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-build && /usr/bin/cmake -E touch /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-build

portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-configure: portaudio-populate-prefix/tmp/portaudio-populate-cfgcmd.txt
portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-configure: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "No configure step for 'portaudio-populate'"
	cd /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-build && /usr/bin/cmake -E echo_append
	cd /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-build && /usr/bin/cmake -E touch /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-configure

portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-download: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-gitinfo.txt
portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-download: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'portaudio-populate'"
	cd /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps && /usr/bin/cmake -P /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/portaudio-populate-prefix/tmp/portaudio-populate-gitclone.cmake
	cd /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps && /usr/bin/cmake -E touch /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-download

portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-install: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'portaudio-populate'"
	cd /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-build && /usr/bin/cmake -E echo_append
	cd /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-build && /usr/bin/cmake -E touch /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-install

portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'portaudio-populate'"
	/usr/bin/cmake -Dcfgdir= -P /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/portaudio-populate-prefix/tmp/portaudio-populate-mkdirs.cmake
	/usr/bin/cmake -E touch /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-mkdir

portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-patch: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'portaudio-populate'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-patch

portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-test: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No test step for 'portaudio-populate'"
	cd /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-build && /usr/bin/cmake -E echo_append
	cd /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-build && /usr/bin/cmake -E touch /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-test

portaudio-populate: CMakeFiles/portaudio-populate
portaudio-populate: CMakeFiles/portaudio-populate-complete
portaudio-populate: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-build
portaudio-populate: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-configure
portaudio-populate: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-download
portaudio-populate: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-install
portaudio-populate: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-mkdir
portaudio-populate: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-patch
portaudio-populate: portaudio-populate-prefix/src/portaudio-populate-stamp/portaudio-populate-test
portaudio-populate: CMakeFiles/portaudio-populate.dir/build.make
.PHONY : portaudio-populate

# Rule to build all files generated by this target.
CMakeFiles/portaudio-populate.dir/build: portaudio-populate
.PHONY : CMakeFiles/portaudio-populate.dir/build

CMakeFiles/portaudio-populate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/portaudio-populate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/portaudio-populate.dir/clean

CMakeFiles/portaudio-populate.dir/depend:
	cd /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild /home/run/文档/PlatformIO/Projects/my_bluetooth_musicbox/lib/audio-tools-0.9.7/_deps/portaudio-subbuild/CMakeFiles/portaudio-populate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/portaudio-populate.dir/depend

