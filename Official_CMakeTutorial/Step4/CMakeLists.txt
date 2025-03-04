# TODO 4: Update the minimum required version to 3.15

cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Tutorial VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TODO 1: Replace the following code by:
# * Creating an interface library called tutorial_compiler_flags
#   Hint: use add_library() with the INTERFACE signature
# * Add compiler feature cxx_std_11 to tutorial_compiler_flags
#   Hint: Use target_compile_features()
add_library(tutorial_compiler_flags INTERFACE)  
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# TODO 5: Create helper variables to determine which compiler we are using:
# * Create a new variable gcc_like_cxx that is true if we are using CXX and
#   any of the following compilers: ARMClang, AppleClang, Clang, GNU, LCC
# * Create a new variable msvc_cxx that is true if we are using CXX and MSVC
# Hint: Use set() and COMPILE_LANG_AND_ID
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# TODO 6: Add warning flag compile options to the interface library
# tutorial_compiler_flags.
# * For gcc_like_cxx, add flags -Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused
# * For msvc_cxx, add flags -W3
# Hint: Use target_compile_options()


# TODO 7: With nested generator expressions, only use the flags for the
# build-tree
# Hint: Use BUILD_INTERFACE

# Generator Expressions in CMake - Explanation
#
# Generator expressions like "$<${gcc_like_cxx}:...>" are evaluated during build system 
# generation, not during CMake configuration. This is an important distinction:
#
# - CMake Configuration Phase (cmake ..): Variables are set, conditions evaluated
#   During this phase, you can check ${CMAKE_CXX_COMPILER_ID} to predict which path will be taken
#   but the generator expressions themselves won't be evaluated yet.
#
# - Build System Generation Phase: Generator expressions are translated into build rules
#   This happens at the end of CMake configuration and during build file generation.
#
# - Build Phase (make/ninja): The previously evaluated expressions are used
#
# How to check which generator expressions evaluated to true:
#
# 1. Use verbose build output:
#    make VERBOSE=1  
#    (or) cmake --build . -v
#    
#    Look for compiler flags in the output. In our case:
#    - If you see -Wall -Wextra -Wshadow -Wformat=2 -Wunused -> gcc_like_cxx is TRUE
#    - If you see -W3 -> msvc_cxx is TRUE
#
# 2. Create a file with expanded expressions:
#    file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/expanded_flags.txt"
#         CONTENT "Compiler flags: $<$<BOOL:${gcc_like_cxx}>:GCC-like compiler detected>\n
#                  $<$<BOOL:${msvc_cxx}>:MSVC compiler detected>\n")
#
# 3. Export compile commands (useful for tooling):
#    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#    Then examine compile_commands.json in the build directory
#
# Note: The BUILD_INTERFACE specifier ensures these flags are only used when building
# directly from this project, not when the project is used from an install location.
target_compile_options(tutorial_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>")

# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the MathFunctions library
if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
endif()

# add the executable
add_executable(Tutorial tutorial.cxx)

# TODO 2: Link to tutorial_compiler_flags
target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS} tutorial_compiler_flags)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
