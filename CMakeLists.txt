cmake_minimum_required( VERSION  3.0 )

project( handh )


set( CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -std=c++2a" )


add_executable( ${PROJECT_NAME} "main.cpp" )
