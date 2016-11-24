cmake_minimum_required (VERSION 2.6)
project (OXYGINE_GOOGLEPLAY)

set(OXYGINE_GOOGLEPLAY_INCLUDE_DIRS 
	${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${OXYGINE_GOOGLEPLAY_INCLUDE_DIRS})
add_library(oxygine-googleplay STATIC 
	src/googleplay.cpp 
	src/googleplay.h
	src/sim/GooglePlaySimulator.cpp 
	src/sim/GooglePlaySimulator.h
)

#target_link_libraries(oxyginemagicparticles)

set(OXYGINE_GOOGLEPLAY_INCLUDE_DIRS ${OXYGINE_GOOGLEPLAY_INCLUDE_DIRS} PARENT_SCOPE)
set(OXYGINE_GOOGLEPLAY_LIBS ${OXYGINE_GOOGLEPLAY_LIBS} PARENT_SCOPE)