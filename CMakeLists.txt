cmake_minimum_required(VERSION 3.16)
project(tensorscript LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/usr/lib/llvm-14/lib/cmake/llvm;/usr/lib/llvm-14/lib/cmake/mlir")

find_package(LLVM 14 REQUIRED CONFIG PATHS "/usr/lib/llvm-14/lib/cmake/llvm" NO_DEFAULT_PATH)
find_package(MLIR REQUIRED CONFIG PATHS "/usr/lib/llvm-14/lib/cmake/mlir" NO_DEFAULT_PATH)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using MLIRConfig.cmake in: ${MLIR_DIR}")

include_directories(include
                    ${LLVM_INCLUDE_DIRS}
                    ${MLIR_INCLUDE_DIRS})

link_directories(${LLVM_LIBRARY_DIRS})

add_executable(tensorscript
                src/lexer.cpp
                src/parser.cpp
                src/sema.cpp
                tests/test_parser.cpp)

target_link_libraries(tensorscript PRIVATE
    MLIRIR
    MLIRParser
    MLIRTensor
    MLIRLinalg
    MLIRArithmetic
    MLIRSupport
)

