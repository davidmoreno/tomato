include_directories (${PROJECT_SOURCE_DIR}/src/) 

add_executable(users users.cpp)

target_link_libraries(users tmtcpp)