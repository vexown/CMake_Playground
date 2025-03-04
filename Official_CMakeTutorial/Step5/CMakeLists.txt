# REMEMBER TO SPECIFY THE INSTALLATION DIRECTORY via -DCMAKE_INSTALL_PREFIX
# Example: cmake .. -DCMAKE_INSTALL_PREFIX=/home/blankmcu/Repos/CMake_Playground/Official_CMakeTutorial/Step5_install
#
# The following commands are used to install the Tutorial project:
#   - install(TARGETS): Installs the Tutorial executable in the bin directory
#   - install(FILES): Installs the TutorialConfig.h header file in the include directory
#
# Then you need to call:
#   - cmake .. -DCMAKE_INSTALL_PREFIX=/home/blankmcu/Repos/CMake_Playground/Official_CMakeTutorial/Step5_install
#   - cmake --build . (or make)
#   - cmake --install . (or make install)
#
# The output of the installation should be in the directory specified with -DCMAKE_INSTALL_PREFIX
#

cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Tutorial VERSION 1.0)

# specify the C++ standard
add_library(tutorial_compiler_flags INTERFACE)

# target_compile_features vs target_compile_options
#
# 1. target_compile_features: Specifies language features needed by your code
#    - High-level abstraction (what you need)
#    - Example: target_compile_features(MyTarget PRIVATE cxx_std_11)
#    - CMake translates this to the appropriate compiler-specific flags
#    - Portable across different compilers and platforms
#    - Used for: C++ standards, specific language features (lambdas, constexpr)
#
# 2. target_compile_options: Specifies direct compiler flags
#    - Low-level implementation (how to achieve it)
#    - Example: target_compile_options(MyTarget PRIVATE -Wall)
#    - Often requires conditional logic for different compilers
#    - Used for: Warning levels, optimization flags, preprocessor definitions
#
# Best Practice:
# - Use target_compile_features for language standard requirements
# - Use target_compile_options for compiler-specific behavior
# - When both could work (like setting C++ standard), prefer target_compile_features
#   for better portability
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(tutorial_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

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
target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS} tutorial_compiler_flags)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

################################################### INSTALLING #############################################

# TODO 3: Install Tutorial in the bin directory
# Hint: Use the TARGETS and DESTINATION parameters
install(TARGETS Tutorial DESTINATION bin)

# TODO 4: Install Tutorial.h to the include directory
# Hint: Use the FILES and DESTINATION parameters
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h" 
        DESTINATION include
        )

################################################### TESTING #############################################

# TODO 5: Enable testing
enable_testing()

# TODO 6: Add a test called Runs which runs the following command:
# $ Tutorial 25 (it runs our executable that we have built with the argument 25)
add_test(NAME Runs COMMAND Tutorial 25)

# TODO 7: Add a test called Usage which runs the following command:
# $ Tutorial
# Make sure the expected output is displayed.
# Hint: Use the PASS_REGULAR_EXPRESSION property with "Usage.*number"
add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
  )

# CMake Testing and Output Verification
#
# 1. add_test(NAME StandardUse COMMAND Tutorial 4)
#    - Creates a test named "StandardUse"
#    - Executes the command "Tutorial 4" (our executable with argument 4)
#
# 2. set_tests_properties with PASS_REGULAR_EXPRESSION:
#    - Defines success criteria for the test
#    - The test PASSES only if the program output contains the specified string
#    - Uses regular expression matching on the program's stdout
#    - If the pattern is not found, the test FAILS
#
# In this case:
# - We run "Tutorial 4" 
# - We expect it to calculate the square root of 4
# - The test passes if "4 is 2" appears in the output
# - This verifies both program execution and correctness of calculation
#
# This approach allows automated verification that:
# - The program runs without crashing
# - The program produces expected output for given inputs
# - Mathematical operations produce correct results
add_test(NAME StandardUse 
        COMMAND Tutorial 4)

set_tests_properties(StandardUse
                        PROPERTIES PASS_REGULAR_EXPRESSION "4 is 2")

# TODO 9: Add more tests. Create a function called do_test to avoid copy +
# paste. Test the following values: 4, 9, 5, 7, 25, -25 and 0.00001.
function(do_test target arg result)
  add_test(NAME Comp${arg} COMMAND ${target} ${arg})
  set_tests_properties(Comp${arg}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction()

# do a bunch of result based tests
do_test(Tutorial 4 "4 is 2")
do_test(Tutorial 9 "9 is 3")
do_test(Tutorial 5 "5 is 2.236")
do_test(Tutorial 7 "7 is 2.645")
do_test(Tutorial 25 "25 is 5")
do_test(Tutorial -25 "-25 is (-nan|nan|0)")
do_test(Tutorial 0.0001 "0.0001 is 0.01")

