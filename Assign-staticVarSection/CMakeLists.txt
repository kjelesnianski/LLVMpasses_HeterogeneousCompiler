cmake_minimum_required(VERSION 2.8)
project(assign-static-var-section)
set(assign-static-var-section_VERSION_MAJOR 1)
set(assign-static-var-section_VERSION_MINOR 0)

find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS} include)

add_library(AssignStaticVarSection MODULE src/AssignStaticVarSection.cpp)

install(TARGETS AssignStaticVarSection DESTINATION lib)
