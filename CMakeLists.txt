# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Tutorials)

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



# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")


if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)


set(ALL_LIBS
		${OPENGL_LIBRARY}
		glfw
		GLEW
		assimp
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# User playground
add_executable(playground
		playground/playground.cpp
		common/shader.cpp
		common/shader.hpp
		common/stb_image.cpp
		common/stb_image.h
		common/camera.cpp
		common/camera.h
		)
target_link_libraries(playground
	${ALL_LIBS}
)

# User playground
add_executable(playground_advanced_opengl
		playground_advanced_opengl/playground.cpp
		common/shader.cpp
		common/shader.hpp
		common/stb_image.cpp
		common/stb_image.h
		common/camera.cpp
		common/camera.h
		)
target_link_libraries(playground_advanced_opengl
		${ALL_LIBS}
		)


# User playground
add_executable(playground_geometry_shader
		playground_geometry_shader/playground.cpp
		common/shader.cpp
		common/shader.hpp
		common/stb_image.cpp
		common/stb_image.h
		common/camera.cpp
		common/camera.h
		)
target_link_libraries(playground_geometry_shader
		${ALL_LIBS}
		)



# User playground
add_executable(playground_load_model
		playground_load_model/playground.cpp
		common/shader.cpp
		common/shader.hpp
		common/stb_image.cpp
		common/stb_image.h
		common/camera.cpp
		common/camera.h
		common/mesh.cpp
		common/mesh.h
		common/model.cpp
		common/model.h)
target_link_libraries(playground_load_model
		${ALL_LIBS}
		)

# User playground
add_executable(playground_load_model_skybox
		playground_load_model_skybox/playground.cpp
		common/shader.cpp
		common/shader.hpp
		common/stb_image.cpp
		common/stb_image.h
		common/camera.cpp
		common/camera.h
		common/mesh.cpp
		common/mesh.h
		common/model.cpp
		common/model.h)
target_link_libraries(playground_load_model_skybox
		${ALL_LIBS}
		)


# User playground
add_executable(playground_math_test
		playground_math_test/calculate.cpp
		)
target_link_libraries(playground_math_test
		${ALL_LIBS}
		)

