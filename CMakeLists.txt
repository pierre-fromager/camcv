cmake_minimum_required(VERSION 3.2)

project(camcv)

add_definitions(
	# -DAE_CPU=AE_INTEL -DAE_NO_AVX2 -DAE_NO_FMA 
    -O2 -Werror -Wall -Wextra -Wpedantic -std=c++17 -g 
	-Wno-unused-function -Wno-unused-parameter -Wno-unused-variable 
	-Wno-format-nonliteral -Wcast-qual -Wno-strict-aliasing 
	-Wformat=2 -Wformat-security 
	-Wno-null-dereference  -Wstack-protector -Wtrampolines -Wvla 
	-Warray-bounds=2 -Wno-maybe-uninitialized
	-Wshift-overflow=2 
	-Wlogical-op -Wduplicated-cond 
	-Wformat-signedness -Wshadow 
	-Wno-undef -Wswitch-enum 
	-Wstack-usage=500000 
	-D_FORTIFY_SOURCE=2 
	-fsanitize=leak
)

file(GLOB ${PROJECT_NAME}_sources 
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(/usr/include/opencv4)

add_definitions (-DBOOST_TEST_DYN_LINK)

find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_sources})

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    ${OpenCV_LIBS}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

