cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(tetris VERSION 0.1.0 LANGUAGES C)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
endif()

# Dependencies
set(RAYLIB_VERSION 4.5.0)
if (NOT ${PLATFORM} STREQUAL "Web")
  set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
endif()
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

# Our Project
if (NOT ${PLATFORM} STREQUAL "Web")
  file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/main.c src/base/*.c src/base/*.h)
  add_executable(${PROJECT_NAME} ${SOURCES})
else()
  file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/main.c src/game/*.c src/game/*.h)
  add_executable(${PROJECT_NAME} ${SOURCES})
endif()
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets )

include_directories(${CMAKE_CURRENT_SOURCE_DIR} src/shared)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG_MODE=1)
  add_compile_definitions(LOG_LEVEL=LOG_DEBUG)
else ()
  add_compile_definitions(LOG_LEVEL=LOG_INFO)
endif()

if (MSVC)
  add_compile_definitions(LOG_LEVEL=LOG_DEBUG)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
endif()

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY --preload-file ${CMAKE_SOURCE_DIR}/assets@/assets/")
    #    Use different shell file
    #    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --shell-file ${CMAKE_SOURCE_DIR}/src/shell.html")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
else()
  if (NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror
    )
    # -fsanitize=address -fno-omit-frame-pointer)
  endif()
endif()

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

if (NOT ${PLATFORM} STREQUAL "Web")
  add_subdirectory(src/game)
endif()