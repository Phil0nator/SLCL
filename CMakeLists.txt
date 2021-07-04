# CMakeList.txt : CMake project for SLCL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(SLCL)

file(GLOB_RECURSE SLCL/src) 
set(socktest examples/src/socktest.c  )


# -- Library --
add_library( SLCL STATIC src/Network.c src/Errors.c src/Signals.c src/Mmap.c  )
target_include_directories(SLCL PUBLIC "include")

# -- Examples --
add_executable( "socktest.exe" ${socktest} )
target_link_libraries( socktest.exe SLCL )

add_executable( "mmaptest.exe" examples/src/mmaptest.c )
target_link_libraries( mmaptest.exe SLCL )

# TODO: Add tests and install targets if needed.
