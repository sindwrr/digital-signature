cmake_minimum_required(VERSION 3.12)
project(exec)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
    main.cpp
    process.cpp
    signature.cpp
)

set(HEADERS
    signature-aux.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})