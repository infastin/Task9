cmake_minimum_required(VERSION 3.1.3)

project(prac CXX)

# Set standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compile commands for lang server
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set GCC compile options
add_compile_options(
	-g -O2 -pipe 
	-D_FORTIFY_SOURCE=2 
	-Wall
	-funsigned-char -fms-extensions
)


set(INCLUDE_DIR include)

set(DIRS 
	${INCLUDE_DIR}
)

# Set libs
add_library(base STATIC
	${INCLUDE_DIR}/String.cpp
	${INCLUDE_DIR}/BufString.cpp
)

target_include_directories(base PUBLIC ${DIRS})

# Exec
set(EXECUTABLES
	main
)

foreach(EXEC IN LISTS EXECUTABLES)
	add_executable(${EXEC} ${EXEC}.cpp)
	target_link_libraries(${EXEC} base)
endforeach()
