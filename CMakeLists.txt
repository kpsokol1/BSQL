cmake_minimum_required(VERSION 3.16)
project(BSQL)

set(CMAKE_CXX_STANDARD 17)

add_executable(BSQL BSQL.cpp BSQL.h global.cpp global.h Scanner.cpp Scanner.h Parser.cpp Parser.h)