cmake_minimum_required(VERSION 3.6.0)
project(IOBench VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(es2 src/main.c)
target_compile_options(es2 PUBLIC -Wall -Wextra)

# link libso library
link_directories(/usr/local/lib/)
target_link_libraries(es2 so)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
