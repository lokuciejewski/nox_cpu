cmake_minimum_required(VERSION 3.27)

project(nox-cpu VERSION 0.2.0)

set(PROJECT_EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples/)

# All cpu operations - rebuild CMakeLists if the list of files change
aux_source_directory(src/components/cpu_ops CPU_OPS)

set(SOURCE_FILES src/components/clock.c src/components/cpu.c src/components/stack.c ${CPU_OPS})

include_directories(src/ src/components/)

add_executable(nox-cpu src/main.c ${SOURCE_FILES})

add_custom_target(nox-asm
    COMMAND cargo install nox_asm --git https://github.com/lokuciejewski/nox_asm --version ${CMAKE_PROJECT_VERSION}
)

add_subdirectory(examples)