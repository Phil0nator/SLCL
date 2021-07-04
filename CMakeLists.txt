# CMakeList.txt : CMake project for SLCL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(SLCL)

file(GLOB_RECURSE SLCL/src) 
file( REAL_PATH examples/src/socktest.c socktest  )

# Add source to this project's executable.
add_library( SLCL STATIC src/Network.c src/Errors.c src/Signals.c )
target_include_directories(SLCL PUBLIC "include")

add_executable( "socktest.exe" ${socktest} )
target_link_libraries( socktest.exe SLCL )

# TODO: Add tests and install targets if needed.
