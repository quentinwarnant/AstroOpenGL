# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Astro)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

set (CMAKE_CXX_STANDARD 17)


# Compile external dependencies 
add_subdirectory (external)

include_directories(
	external/glfw-3.3.4/include/
	external/glm-0.9.9.8/
	external/glew-2.1.0/include/
	src/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_210
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# User playground
add_executable(Astro 
	
	# Entry point
	src/main.cpp

	# Windowing system
	src/Window/AWindow.cpp src/Window/AWindow.h

	# Game framework
	src/GameFramework/AstroGame.cpp src/GameFramework/AstroGame.h

	# Voxel
	src/Voxel/Voxel.cpp src/Voxel/Voxel.h
)

# Xcode and Visual working directories
set_target_properties(Astro PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_link_libraries(Astro PUBLIC glfw)
target_link_libraries(Astro PUBLIC GLEW_210)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
