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
  pthread
)

SET(SOURCE_CODE
  main.cpp
  protocol.cpp
  ChunkedMessage.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -O3")

add_executable(gelf-load-balance ${SOURCE_CODE})

target_link_libraries(gelf-load-balance
  ${LINK_LIBRARIES}
)

install(
  TARGETS gelf-load-balance
  RUNTIME DESTINATION bin
)
