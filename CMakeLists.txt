cmake_minimum_required(VERSION 3.5)
project(EpitaClangTidyPlugin)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

# Add the clang-tidy include directory
include_directories(/usr/lib/llvm-14/include)

add_library(EpitaClangTidyPlugin SHARED
    EpitaFunctionArgsCountCheck.cpp
    FunctionSizeCheck.cpp
    EpitaSwitchCaseIndentationCheck.cpp
)

# Link with Clang and LLVM libraries
target_include_directories(EpitaClangTidyPlugin PRIVATE ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS} /usr/lib/llvm-14/include)
target_link_libraries(EpitaClangTidyPlugin PRIVATE clangASTMatchers clangTidy clangTooling)
set_target_properties(EpitaClangTidyPlugin PROPERTIES CXX_STANDARD 17)

