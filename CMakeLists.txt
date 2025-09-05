cmake_minimum_required(VERSION 3.0)
project(mellon-cli)

set(CMAKE_CXX_STANDARD 14)

add_executable(mellon-cli main.cpp Mellon.cpp Mellon.h Commands.h Serial.cpp Serial.h HelperFunctions.cpp HelperFunctions.h Io.cpp Io.h)

install(TARGETS mellon-cli RUNTIME DESTINATION bin)
