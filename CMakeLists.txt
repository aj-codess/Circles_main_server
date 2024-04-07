cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_TOOLCHAIN_FILE /Users/macbook/Documents/Projects/server_engine/vcpkg/scripts/buildsystems/vcpkg.cmake)

project(Circles_Server_Engine VERSION 1.0)

add_executable(main main.cpp)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Boost)
target_include_directories(main PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(main PUBLIC ${Boost_LIBRARIES} ${Boost_DEPENDENCIES})

find_package(Crow CONFIG REQUIRED)
target_include_directories(main PUBLIC ${Crow_INCLUDE_DIR})
target_link_libraries(main PUBLIC ${Crow_LIBRARIES} ${Crow_DEPENDENCIES})

find_package(ZLIB REQUIRED)
target_include_directories(main PUBLIC ${ZLIB_INCLUDE_DIR})
target_link_libraries(main PUBLIC ${ZLIB_LIBRARIES} ${ZLIB_DEPENDENCIES})

find_package(nlohmann_json REQUIRED)
target_include_directories(main PUBLIC ${nlohmann_json_INCLUDE_DIR})
target_link_libraries(main PUBLIC ${nlohmann_json_LIBRARIES} ${nlohmann_json_DEPENDENCIES})
