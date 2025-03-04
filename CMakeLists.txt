cmake_minimum_required(VERSION 3.15)
project(scalf-compiler
  LANGUAGES C CXX
  DESCRIPTION "SCALF Compiler"
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM include dir: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM libraries: ${LLVM_LIBRARY_DIRS}")

# Add LLVM definitions and include directories
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

# Find Flex and Bison
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# Generate lexer and parser
flex_target(SCALF_LEX
  ${PROJECT_SOURCE_DIR}/src/lexer/scalf_lex.l
  ${CMAKE_CURRENT_BINARY_DIR}/scalf_lex.cpp
)

bison_target(SCALF_PARSER
  ${PROJECT_SOURCE_DIR}/src/parser/scalf_parse.y
  ${CMAKE_CURRENT_BINARY_DIR}/scalf_parse.cpp
  COMPILE_FLAGS "-Wno-deprecated"
)

# Add generated files to source list
add_flex_bison_dependency(SCALF_LEX SCALF_PARSER)

# Source files
set(SOURCES
  src/main.cpp
  src/ast/ASTNodes.cpp
  src/sema/SemanticAnalyzer.cpp
  src/codegen/CodeGenerator.cpp
  src/runtime/Runtime.cpp
)

# Include directories
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}  # For generated parser/lexer headers
)

# Create compiler executable
add_executable(scalf-compiler
  ${SOURCES}
  ${FLEX_SCALF_LEX_OUTPUTS}
  ${BISON_SCALF_PARSER_OUTPUTS}
)

# Link LLVM components
llvm_map_components_to_libnames(LLVM_LIBS
  Core
  Support
  Analysis
  ExecutionEngine
  MC
  X86CodeGen  # Adjust for your target architecture
)

# Link dependencies
target_link_libraries(scalf-compiler
  PRIVATE
  ${LLVM_LIBS}
  z
)

# Enable testing
enable_testing()
if(EXISTS "${PROJECT_SOURCE_DIR}/tests")
  add_subdirectory(tests)
endif()

# Installation (optional)
install(TARGETS scalf-compiler DESTINATION bin)