cmake_minimum_required(VERSION 2.8)
project(Chip8Emulator)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Release)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)


file( GLOB_RECURSE SRC_LIST chip8/* )

add_executable(${PROJECT_NAME} ${SRC_LIST})


# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()