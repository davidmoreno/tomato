
include_directories(${PROJECT_SOURCE_DIR}/src/)
add_library(tmtcpp SHARED tmt.cpp queryset.cpp sqlite3.cpp)
add_library(tmtcpp_static STATIC tmt.cpp queryset.cpp sqlite3.cpp)

target_link_libraries(tmtcpp sqlite3)