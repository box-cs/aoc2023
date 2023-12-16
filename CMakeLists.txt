cmake_minimum_required(VERSION 3.7)
project(aoc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(ExternalProject)
file(DOWNLOAD
   https://raw.githubusercontent.com/anactualfox/simpleio/master/io.hpp
   ${CMAKE_SOURCE_DIR}/lib/io.hpp)


file(GLOB MAIN_FILES "**/*.cpp")

if (DEFINED TARGET_NAME) 
   message(STATUS "TARGET_NAME: ${TARGET_NAME}")
else()   
   message(FATAL_ERROR "TARGET_NAME variable is not defined.")
endif()

foreach(MAIN_FILE ${MAIN_FILES})
   get_filename_component(EXE_NAME ${MAIN_FILE} NAME_WE)
   get_filename_component(DIRECTORY ${MAIN_FILE} NAME_WE)
   if (EXE_NAME STREQUAL ${TARGET_NAME})
      message(STATUS "Adding executable: ${EXE_NAME}")
         add_executable(${EXE_NAME} ${MAIN_FILE})
            file (GLOB INPUT_FILES "${DIRECTORY}/*.txt")
            foreach(INPUT_FILE ${INPUT_FILES})
               get_filename_component(TXT_NAME ${INPUT_FILE} NAME_WE)
               configure_file(${INPUT_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${TXT_NAME}.txt COPYONLY)
            endforeach()
   else()
      message(STATUS "Skipping executable: ${EXE_NAME}")
   endif()
endforeach()
