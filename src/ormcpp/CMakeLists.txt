
include_directories(${PROJECT_SOURCE_DIR}/src/)
add_library(ormcpp SHARED orm.cpp queryset.cpp sqlite3.cpp)
add_library(ormcpp_static STATIC orm.cpp queryset.cpp sqlite3.cpp)

target_link_libraries(ormcpp sqlite3)