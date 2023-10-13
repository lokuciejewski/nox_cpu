cmake_minimum_required(VERSION 3.27)

project(nox-cpu)

set(PROJECT_EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples/)

set(SOURCE_FILES src/components/clock.c src/components/cpu.c src/components/stack.c)

include_directories(src/ src/components/)

add_executable(nox-cpu src/main.c ${SOURCE_FILES})

add_custom_target(nox-asm
    # COMMAND cargo install nox-asm --git https://github.com/lokuciejewski/nox_asm
    COMMAND cargo install --path /home/anemiq/Projects/nox_asm
)

add_custom_target(examples_print_string
    DEPENDS nox-asm
    DEPENDS nox-cpu
    WORKING_DIRECTORY ${PROJECT_EXAMPLES_DIR}/print_string
    COMMAND nox_asm -i print_string.nox -o memory.bin 
    VERBATIM
)

add_custom_command(TARGET examples_print_string
    POST_BUILD
    DEPENDS ${PROJECT_EXAMPLES_DIR}/print_string/memory.bin
    COMMAND echo "++++ CPU OUTPUT ++++"
    COMMAND nox-cpu ${PROJECT_EXAMPLES_DIR}/print_string/memory.bin 100
    COMMAND echo -e "\\n++++ CPU OUTPUT END ++++"
    VERBATIM
)

add_custom_target(examples_fibonacci
    DEPENDS nox-asm
    DEPENDS nox-cpu
    WORKING_DIRECTORY ${PROJECT_EXAMPLES_DIR}/fibonacci
    COMMAND nox_asm -i fibonacci.nox -o memory.bin
    VERBATIM
)

add_custom_command(TARGET examples_fibonacci
    POST_BUILD
    DEPENDS ${PROJECT_EXAMPLES_DIR}/fibonacci/memory.bin
    COMMAND echo "++++ CPU OUTPUT ++++"
    COMMAND nox-cpu ${PROJECT_EXAMPLES_DIR}/fibonacci/memory.bin 100
    COMMAND echo -e "\\n++++ CPU OUTPUT END ++++"
    VERBATIM
)