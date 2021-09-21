# ibc
add_executable(ibc
                Main.cpp
                Ibc.cpp)

target_include_directories(ibc
                            PRIVATE ${INTUITIVE_BLOCKCHAIN_INCLUDE_DIRS})

# compile options
target_compile_options(ibc PUBLIC ${COMPILE_OPTIONS})

# compile definitions
target_compile_definitions(ibc PUBLIC ${COMPILE_DEFINITIONS})
