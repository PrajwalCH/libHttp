cmake_minimum_required(VERSION 2.8)
project(libHttp)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)

add_executable(
    main
    src/main.c
    src/request_parser.c
)

