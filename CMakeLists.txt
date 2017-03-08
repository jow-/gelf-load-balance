project(gelf-load-balance)

cmake_minimum_required(VERSION 2.8)

INCLUDE_DIRECTORIES(
  /usr/inlcude
  /usr/local/include
)

LINK_DIRECTORIES(
  /usr/lib
  /usr/local/lib
) 

SET(LINK_LIBRARIES
  stdc++
  boost_system
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14 -g")

add_executable(gelf-load-balance main.cpp)

target_link_libraries(gelf-load-balance
  ${LINK_LIBRARIES}
)