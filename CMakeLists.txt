# CMake entry point
cmake_minimum_required (VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
project (Tutorials)

set(OpenGL_GL_PREFERENCE LEGACY)

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


if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)


set(ALL_LIBS
		${OPENGL_LIBRARY}
		glfw
		GLEW
		assimp
)

set(src
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

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# User playground
add_executable(playground
		${src}
		playground/playground.cpp
		)
target_link_libraries(playground
	${ALL_LIBS}
)

# User playground
add_executable(playground_advanced_opengl
		playground_advanced_opengl/playground.cpp
		${src}
		)
target_link_libraries(playground_advanced_opengl
		${ALL_LIBS}
		)


# User playground
add_executable(playground_geometry_shader
		playground_geometry_shader/playground.cpp
		${src}
		)
target_link_libraries(playground_geometry_shader
		${ALL_LIBS}
		)


# User playground
add_executable(playground_load_model
		playground_load_model/playground.cpp
		${src})
target_link_libraries(playground_load_model
		${ALL_LIBS}
		)


# User playground
add_executable(playground_instancing_planet
		playground_instancing_planet/playground.cpp
		${src})
target_link_libraries(playground_instancing_planet
		${ALL_LIBS}
		)

# User playground
add_executable(playground_instancing
		playground_instancing/playground.cpp
		${src})
target_link_libraries(playground_instancing
		${ALL_LIBS}
		)

# User playground
add_executable(playground_load_model_skybox
		playground_load_model_skybox/playground.cpp
		${src})
target_link_libraries(playground_load_model_skybox
		${ALL_LIBS}
		)

# User playground
add_executable(playground_multisample_anti_aliasing
		playground_multisample_anti_aliasing/playground.cpp
		${src})
target_link_libraries(playground_multisample_anti_aliasing
		${ALL_LIBS}
		)

# playground_blinn_phong
add_executable(playground_blinn_phong
		playground_blinn_phong/playground.cpp
		${src})
target_link_libraries(playground_blinn_phong
		${ALL_LIBS}
		)

# playground_gamma_correction
add_executable(playground_gamma_correction
		playground_gamma_correction/playground.cpp
		${src})
target_link_libraries(playground_gamma_correction
		${ALL_LIBS}
		)

# playground_shadow_mapping
add_executable(playground_shadow_mapping
		playground_shadow_mapping/playground.cpp
		${src})
target_link_libraries(playground_shadow_mapping
		${ALL_LIBS}
		)

# playground_shadow_mapping_point_light
add_executable(playground_shadow_mapping_point_light
		playground_shadow_mapping_point_light/playground.cpp
		${src})
target_link_libraries(playground_shadow_mapping_point_light
		${ALL_LIBS}
		)

# playground_normal_mapping
add_executable(playground_normal_mapping
		playground_normal_mapping/playground.cpp
		${src})
target_link_libraries(playground_normal_mapping
		${ALL_LIBS}
		)

# playground_parrallax_mapping
add_executable(playground_parrallax_mapping
		playground_parrallax_mapping/playground.cpp
		${src})
target_link_libraries(playground_parrallax_mapping
		${ALL_LIBS}
		)

# playground_HDR
add_executable(playground_HDR
		playground_HDR/playground.cpp
		${src})
target_link_libraries(playground_HDR
		${ALL_LIBS}
		)

# playground_hdr_bloom
add_executable(playground_hdr_bloom
		playground_hdr_bloom/playground.cpp
		${src})
target_link_libraries(playground_hdr_bloom
		${ALL_LIBS}
		)

# playground_deferred_shading
add_executable(playground_deferred_shading
		playground_deferred_shading/playground.cpp
		${src})
target_link_libraries(playground_deferred_shading
		${ALL_LIBS}
		)

# User playground
add_executable(playground_math_test
		playground_math_test/calculate.cpp
		playground_math_test/filesys.cpp playground_math_test/TBN.cpp playground_math_test/normal_matrix.cpp)
target_link_libraries(playground_math_test
		${ALL_LIBS}
		)


# opengl 460 core

# opengl reference guide ep1
# User playground
add_executable(opg_1.1_triangles
		${src}
		opg_1.1_triangles/playground.cpp
		)
target_link_libraries(opg_1.1_triangles
		${ALL_LIBS}
		)


add_executable(opg_1.2_uniform_block
		${src}
		opg_1.2_uniform_block/playground.cpp
		)
target_link_libraries(opg_1.2_uniform_block
		${ALL_LIBS}
		)

add_executable(opg_1.3_program_pipeline
		${src}
		opg_1.3_program_pipeline/playground.cpp
		)
target_link_libraries(opg_1.3_program_pipeline
		${ALL_LIBS}
		)

add_executable(opg_3.1_diffuse_lighting
		${src}
		opg_3.1_diffuse_lighting/playground.cpp
		)
target_link_libraries(opg_3.1_diffuse_lighting
		${ALL_LIBS}
		)























