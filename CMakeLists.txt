cmake_minimum_required(VERSION 3.23)
cmake_policy(VERSION 3.23...3.30)

# Define the Project
project(Denoise VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)
# Include the CMake Package Manager
set(FFTW_VERSION 3.3.10)
include(cmake/get_cpm.cmake)
include(cmake/setupFFTW.cmake)
# download CImg with CPM download only clause
CPMAddPackage(
  NAME CImg
  GITHUB_REPOSITORY /GreycLab/CImg
  VERSION .3.4.0
  DOWNLOAD_ONLY YES
)

file(DOWNLOAD https://people.sc.fsu.edu/~jburkardt/cpp_src/image_denoise/balloons_noisy.png ${CMAKE_CURRENT_BINARY_DIR}/bin/input.png)

find_package(Threads REQUIRED)

if(CImg_ADDED)
  add_library(CImg INTERFACE)
  target_include_directories(CImg INTERFACE ${CImg_SOURCE_DIR})
  target_link_libraries(CImg INTERFACE Threads::Threads)
endif()

add_executable(main main.cpp)
target_link_libraries(main PRIVATE CImg ${FFTW_LIBRARIES})
