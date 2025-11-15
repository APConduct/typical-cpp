cmake_minimum_required(VERSION 3.28)

# Add example executable
add_executable(example_01 main.cpp)

# Link against the typical library
target_link_libraries(example_01 PRIVATE typical)

# Set C++ standard
set_target_properties(example_01 PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
