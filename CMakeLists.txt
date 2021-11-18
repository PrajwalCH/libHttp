cmake_minimum_required(VERSION 2.8)
project(libHttp)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_C_FLAGS_DEBUG "-g -ggdb")

set(SOURCES
    src/header_vector.c
    src/main.c
    src/request_builder.c
    src/request_dealloc.c
    src/request_parser.c
    src/sds.c
)

add_executable(
    main
    ${SOURCES}
)

