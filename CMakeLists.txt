cmake_minimum_required(VERSION 3.27)

project(nox-cpu)

set(SOURCE_FILES src/components/clock.c src/components/cpu.c src/components/stack.c)

include_directories(src/ src/components/)

add_executable(nox-cpu src/main.c ${SOURCE_FILES})