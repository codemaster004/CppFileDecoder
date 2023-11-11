cmake_minimum_required(VERSION 3.26)
project(Base64FileDecoder)

set(CMAKE_CXX_STANDARD 17)

add_executable(Base64FileDecoder src/main.cpp
        src/PrintingFunctions.h
        src/PrintingFunctions.cpp
        src/DecodedCharHolder.cpp
        src/DecodedCharHolder.h
        src/tarFileHeader.cpp
        src/tarFileHeader.h
)
