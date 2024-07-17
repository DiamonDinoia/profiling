cmake_minimum_required(VERSION 3.9)
cmake_policy(VERSION 3.9...3.30)

# Define the Project
project(Denoise VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)


# set the build type if not set
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif ()

# Include the CMake Package Manager
set(FFTW_VERSION 3.3.10)
include(cmake/get_cpm.cmake)
include(cmake/setupFFTW.cmake)
include(cmake/setupGperfTools.cmake)
include(cmake/setupCImg.cmake)
# download CImg with CPM download only clause
file(DOWNLOAD https://people.sc.fsu.edu/~jburkardt/cpp_src/image_denoise/balloons_noisy.png ${CMAKE_BINARY_DIR}/bin/input.png)

find_package(Threads REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE CImg profiler)
# this is needed to have nice stack traces in profiling tools
target_compile_options(main PRIVATE -fno-omit-frame-pointer -momit-leaf-frame-pointer -g3 -gdwarf-4 -march=native)
#-fno-omit-frame-pointer: This option disables the omission of the frame pointer (FP). Normally, compilers might omit
#   the frame pointer for optimization purposes (to free up a register for other uses), but keeping the frame pointer
#   makes debugging easier because it maintains a chain of stack frames that can be followed.
#-momit-leaf-frame-pointer: This option allows the compiler to omit the frame pointer for leaf functions
#   (functions that do not call any other functions). This is a middle ground that can provide some of the benefits
#   of omitting the frame pointer (such as additional register for optimizations) while still retaining some debuggability
#   for non-leaf functions.
#-g3: This flag controls the debug information level in the generated binaries. -g3 includes extra debug information,
#   such as macro definitions, which can be useful for intensive debugging sessions.
#   It provides more information than -g2 or -g, potentially at the cost of larger debug files.
#-gdwarf-4: This specifies the format and version of the debug information. DWARF is a widely used format for debug data.
#   -gdwarf-4 specifies using version 4 of the DWARF standard, which includes various improvements and optimizations
#   over earlier versions. This flag ensures compatibility with debuggers and other tools that understand DWARF-4 format.

#add sanitisers in debug mode
target_compile_options(main PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address -fsanitize=undefined -fsanitize=bounds-strict>)
target_link_options(main PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address -fsanitize=undefined -fsanitize=bounds-strict>)

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
