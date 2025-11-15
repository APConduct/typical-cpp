cmake_minimum_required(VERSION 3.28)
project(typical)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)



set(CMAKE_CXX_STANDARD 20)

add_library(typical STATIC
    include/typical.hpp)

target_sources(typical
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
    include/modules/typical.ixx
    include/modules/typical/lambda.ixx
    include/modules/typical/church.ixx
    include/modules/typical/eq.ixx
    include/modules/typical/set.ixx
    include/modules/typical/nat.ixx
    include/modules/typical/calculus.ixx
    include/modules/typical/refine.ixx
)

# Enable testing
enable_testing()

# Add tests subdirectory
add_subdirectory(tests)

# Add examples subdirectory
add_subdirectory(examples)
