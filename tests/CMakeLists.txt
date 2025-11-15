cmake_minimum_required(VERSION 3.28)

# Add test executable for basic lambda tests
add_executable(lambda_tests lambda_tests.cpp)

# Link against the typical library
target_link_libraries(lambda_tests PRIVATE typical)

# Set C++ standard
set_target_properties(lambda_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Add the test
add_test(NAME lambda_tests COMMAND lambda_tests)

# Add test executable for advanced lambda tests
add_executable(lambda_advanced_tests lambda_advanced_tests.cpp)

# Link against the typical library
target_link_libraries(lambda_advanced_tests PRIVATE typical)

# Set C++ standard
set_target_properties(lambda_advanced_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Add the advanced test
add_test(NAME lambda_advanced_tests COMMAND lambda_advanced_tests)

# Add test executable for church module tests
add_executable(church_tests church_tests.cpp)

# Link against the typical library
target_link_libraries(church_tests PRIVATE typical)

# Set C++ standard
set_target_properties(church_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Add the church test
add_test(NAME church_tests COMMAND church_tests)

# Add test executable for nat module tests
add_executable(nat_tests nat_tests.cpp)

# Link against the typical library
target_link_libraries(nat_tests PRIVATE typical)

# Set C++ standard
set_target_properties(nat_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Add the nat test
add_test(NAME nat_tests COMMAND nat_tests)
