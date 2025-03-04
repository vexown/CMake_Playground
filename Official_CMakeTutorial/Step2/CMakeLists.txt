cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Tutorial VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# TODO 7: Create a variable USE_MYMATH using option and set default to ON
option(USE_MATH_LIBRARY "Variable to enable/disable MathFunctions library" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# TODO 8: Use list() and APPEND to create a list of optional libraries
# called  EXTRA_LIBS and a list of optional include directories called
# EXTRA_INCLUDES. Add the MathFunctions library and source directory to
# the appropriate lists.
#
# Only call add_subdirectory and only add MathFunctions specific values
# to EXTRA_LIBS and EXTRA_INCLUDES if USE_MYMATH is true.
if(USE_MATH_LIBRARY)
    # TODO 2: Use add_subdirectory() to add MathFunctions to this project
    add_subdirectory(MathFunctions)

    list(APPEND OPTIONAL_LIBS MathFunctions)
    list(APPEND OPTIONAL_INCLUDES MathFunctions)
endif()

# add the executable
add_executable(Tutorial tutorial.cxx)

# TODO 9: Use EXTRA_LIBS instead of the MathFunctions specific values
# in target_link_libraries.
# TODO 3: Use target_link_libraries to link the library to our executable
target_link_libraries(Tutorial PUBLIC 
                      ${OPTIONAL_LIBS}
                      )
# TODO 4: Add MathFunctions to Tutorial's target_include_directories()
# Hint: ${PROJECT_SOURCE_DIR} is a path to the project source. AKA This folder!

# TODO 10: Use EXTRA_INCLUDES instead of the MathFunctions specific values
# in target_include_directories.

target_include_directories(Tutorial PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            ${OPTIONAL_INCLUDES}
                            )

# Lists in CMake - Quick Reference
#
# 1. Lists are created in two ways:
#    a. Space-separated items: set(my_list item1 item2 item3)
#    b. Semicolon-separated string: set(my_list "item1;item2;item3")
#
# 2. When displayed with message(), lists appear as semicolon-separated:
#    - set(var a b c d e) → "${var}" prints as "a;b;c;d;e"
#
# 3. Common list operations:
#    - list(LENGTH list_var output_var)      # Get number of elements
#    - list(GET list_var 0 output_var)       # Get first element (zero-indexed)
#    - list(APPEND list_var new_item)        # Add items to end
#    - list(INSERT list_var 2 new_item)      # Insert at position
#    - list(REMOVE_ITEM list_var item)       # Remove by value
#    - list(REMOVE_AT list_var 2)            # Remove by position
#    - list(JOIN list_var ", " output_var)   # Join with separator
#
# 4. Important distinction:
#    - set(var a b c d e)    # Creates a list with 5 separate items
#    - set(var2 "a b c d e") # Creates a single string with spaces
#
# Example below demonstrates the difference between a list and a quoted string:
set(var a b c d e)
message("var: ${var}")

set(var2 "a b c d e")
message("var2: ${var2}")
