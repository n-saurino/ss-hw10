cmake_minimum_required(VERSION 3.5.0)
project(ss-hw10 VERSION 0.1.0 LANGUAGES C CXX)


# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)  # Use C++17, you can change this to 11, 14, 20, etc.
set(CMAKE_CXX_STANDARD_REQUIRED True)  # Ensure the compiler enforces the standard
set(CMAKE_CXX_EXTENSIONS OFF)  # Disable compiler-specific extensions

# Find Boost
set(BOOST_ROOT "/opt/homebrew/Cellar/boost/1.85.0")
find_package(Boost 1.85.0 REQUIRED COMPONENTS system filesystem)

if(Boost_FOUND)
    message(STATUS "Boost version: ${Boost_VERSION}")
    message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost library dirs: ${Boost_LIBRARY_DIRS}")
else()
    message(FATAL_ERROR "Could not find Boost")
endif()

# Include Boost headers globally
include_directories(${Boost_INCLUDE_DIRS})

add_executable(ss-hw10 main.cpp ex1/ex1.h ex1/Server.h)

# Link Boost libraries
target_link_libraries(ss-hw10 PRIVATE ${Boost_LIBRARIES})