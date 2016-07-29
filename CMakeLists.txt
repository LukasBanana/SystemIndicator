
# === CMake lists for the SystemIndicator - (29/07/2016) ===

cmake_minimum_required(VERSION 2.8)
project(SystemIndicator)


# === Build path ===

set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${OUTPUT_DIR} CACHE PATH "Build directory" FORCE)
set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(PROJECT_TEST_DIR "${PROJECT_SOURCE_DIR}/test")


# === Global files ===

file(GLOB Headers "${PROJECT_SOURCE_DIR}/include/*.*")

if(WIN32)
	file(GLOB SourcesPlatform "${PROJECT_SOURCE_DIR}/sources/Win32/*.*")
elseif(APPLE)
	file(GLOB SourcesPlatform "${PROJECT_SOURCE_DIR}/sources/MacOS/*.*")
elseif(UNIX)
	file(GLOB SourcesPlatform "${PROJECT_SOURCE_DIR}/sources/Linux/*.*")
endif()

set(FilesAll ${Headers} ${SourcesPlatform})


# === Include directories ===

include_directories(${PROJECT_INCLUDE_DIR})


# === Source groups ===

source_group("Include" FILES ${Headers})
source_group("Sources" FILES ${SourcesPlatform})


# === Binaries ===

add_library(SystemIndicator STATIC ${FilesAll})
set_target_properties(SystemIndicator PROPERTIES LINKER_LANGUAGE CXX DEBUG_POSTFIX "D")


# === Test Projects ===

add_executable(Test "${PROJECT_TEST_DIR}/Test.cpp")
set_target_properties(Test PROPERTIES LINKER_LANGUAGE CXX DEBUG_POSTFIX "D")
target_link_libraries(Test SystemIndicator)


