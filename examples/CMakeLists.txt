# Print string
add_custom_target(examples_print_string
    DEPENDS nox-asm
    DEPENDS nox-cpu
    WORKING_DIRECTORY ${PROJECT_EXAMPLES_DIR}/
    COMMAND nox_asm -i print_string.nox -o memory.bin 
    VERBATIM
)

add_custom_command(TARGET examples_print_string
    POST_BUILD
    DEPENDS ${PROJECT_EXAMPLES_DIR}/memory.bin
    COMMAND echo "++++ CPU OUTPUT ++++"
    COMMAND nox-cpu -m ${PROJECT_EXAMPLES_DIR}/memory.bin -d 10
    COMMAND echo -e "\\n++++ CPU OUTPUT END ++++"
    VERBATIM
)

# Fibonacci
add_custom_target(examples_fibonacci
    DEPENDS nox-asm
    DEPENDS nox-cpu
    WORKING_DIRECTORY ${PROJECT_EXAMPLES_DIR}/
    COMMAND nox_asm -i fibonacci.nox -o memory.bin
    VERBATIM
)

add_custom_command(TARGET examples_fibonacci
    POST_BUILD
    DEPENDS ${PROJECT_EXAMPLES_DIR}/memory.bin
    COMMAND echo "++++ CPU OUTPUT ++++"
    COMMAND nox-cpu -m ${PROJECT_EXAMPLES_DIR}/memory.bin -d 10
    COMMAND echo -e "\\n++++ CPU OUTPUT END ++++"
    VERBATIM
)

# Return OK/ERR
add_custom_target(examples_return_ok_err
    DEPENDS nox-asm
    DEPENDS nox-cpu
    WORKING_DIRECTORY ${PROJECT_EXAMPLES_DIR}/
    COMMAND nox_asm -i return_ok_err.nox -o memory.bin
    VERBATIM
)

add_custom_command(TARGET examples_return_ok_err
    POST_BUILD
    DEPENDS ${PROJECT_EXAMPLES_DIR}/memory.bin
    COMMAND echo "++++ CPU OUTPUT ++++"
    COMMAND nox-cpu -m ${PROJECT_EXAMPLES_DIR}/memory.bin -d 10
    COMMAND echo -e "\\n++++ CPU OUTPUT END ++++"
    VERBATIM
)

# Return exit code
add_custom_target(examples_return_exit_code
    DEPENDS nox-cpu
    WORKING_DIRECTORY ${PROJECT_EXAMPLES_DIR}/
    COMMAND nox_asm -i return_exit_code.nox -o memory.bin
    VERBATIM
)

add_custom_command(TARGET examples_return_exit_code
    POST_BUILD
    DEPENDS ${PROJECT_EXAMPLES_DIR}/memory.bin
    COMMAND echo "++++ CPU OUTPUT ++++"
    COMMAND nox-cpu -m ${PROJECT_EXAMPLES_DIR}/memory.bin -d 10
    COMMAND echo -e "\\n++++ CPU OUTPUT END ++++"
    VERBATIM
)

