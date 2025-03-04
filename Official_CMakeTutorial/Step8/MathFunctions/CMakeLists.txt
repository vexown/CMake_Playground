
add_executable(MakeTable MakeTable.cxx)

# Code Generation with add_custom_command
#
# This creates a build-time code generation step that:
# 1. Runs the MakeTable executable we just defined (but only after it has been built first)
# 2. Generates a header file (Table.h) in the build directory
# 3. Makes the build system aware of this generated file
#
# Parameters explained:
# - OUTPUT: The file that will be created by this command
# - COMMAND: The command to execute (MakeTable with Table.h path as argument)
# - DEPENDS: Prerequisites that must be built before running this command
#
# This technique allows us to create a lookup table at build time
# rather than computing values at runtime, improving the performance
# of our square root function.
#
# The generated header is automatically included in the build process
# when it's listed as a source file in add_library() below.
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  DEPENDS MakeTable
  )

message("current binary dir: " ${CMAKE_CURRENT_BINARY_DIR})
message("current source dir: " ${CMAKE_CURRENT_SOURCE_DIR})

add_library(MathFunctions 
            mysqrt.cxx
            ${CMAKE_CURRENT_BINARY_DIR}/Table.h
            )

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          PRIVATE   ${CMAKE_CURRENT_BINARY_DIR}
          )

# link our compiler flags interface library
target_link_libraries(MathFunctions tutorial_compiler_flags)

# install libs
set(installable_libs MathFunctions tutorial_compiler_flags)
install(TARGETS ${installable_libs} DESTINATION lib)
# install include headers
install(FILES MathFunctions.h DESTINATION include)
