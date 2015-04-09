cmake_minimum_required(VERSION 2.8)

project (OpenGL_Program)

find_package(OpenGL REQUIRED)

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})

set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(PROJECT_EXTERNAL_DIR ${CMAKE_BINARY_DIR}/bin/external)

add_subdirectory (external)

include_directories(
    external/glfw-3.0.3/include/
    external/glm-0.9.4.0/
    external/glew-1.9.0/include/
    .
)

set(ALL_LIBS
    ${OPENGL_LIBRARY}
    GLFW_303
    GLEW_190
)

add_definitions(
    -DTW_STATIC
    -DTW_NO_LIB_PRAGMA
    -DTW_NO_DIRECT3D
    -DGLEW_STATIC
    -D_CRT_SECURE_NO_WARNINGS
)

include(src/CMakeLists.txt)

# First Triangle
add_executable(first_triangle 
    ${SRC_FIRST_TRIANGLE}
)
target_link_libraries(first_triangle 
    ${ALL_LIBS}
)

# MVP Matrices
add_executable(mvp_matrices
    ${SRC_MVP_MATRICEX}
)
target_link_libraries(mvp_matrices  
    ${ALL_LIBS}
)

# Cube
add_executable(cube
    ${SRC_CUBE}
)
target_link_libraries(cube
    ${ALL_LIBS}
)

# Cube Move
add_executable(cube_move
    ${SRC_CUBE_MOVE}
)
target_link_libraries(cube_move
    ${ALL_LIBS}
)

# Motion Blur
add_executable(motion_blur
    ${SRC_MOTION_BLUR}
)
target_link_libraries(motion_blur
    ${ALL_LIBS}
)

# Camera
add_executable(camera
    ${SRC_Camera}
)
target_link_libraries(camera
    ${ALL_LIBS}
)

# Multibuffer
add_executable(multibuffer
    ${SRC_Multibuffer}
)
target_link_libraries(multibuffer
    ${ALL_LIBS}
)

SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )
