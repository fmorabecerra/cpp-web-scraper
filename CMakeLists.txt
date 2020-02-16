cmake_minimum_required(VERSION 3.0.0)
project(example1 VERSION 0.1.0)

include(CTest)
enable_testing()

# add the MathFunctions library
add_subdirectory(HTTPDownloader)

add_executable(example1 main.cpp)
target_link_libraries(example1 PUBLIC HTTPDownloader)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)