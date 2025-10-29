cmake_minimum_required(VERSION 3.25.1)

project(users)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(users
  src/main.cc
)

find_package(mariadb-connector-cpp REQUIRED)

target_link_libraries(users
  PRIVATE
  mariadb-connector-cpp::mariadb-connector-cpp
)
