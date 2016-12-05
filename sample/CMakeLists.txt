cmake_minimum_required (VERSION 2.6)
project(dbsample)

file(GLOB src *.cpp *.hpp ../src/*.cpp)

include_directories("../include")

include_directories("/opt/local/include")
link_directories("/opt/local/lib")

include_directories("/usr/local/include")
link_directories("/usr/local/lib")

add_executable(dbsample ${src})

target_link_libraries( dbsample
	boost_thread-mt
	boost_system-mt
	
    sqlite3
)

