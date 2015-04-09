# First Triangle
set(SRC_FIRST_TRIANGLE
	src/first_triangle/main.cpp
	src/first_triangle/Shader.h
	src/first_triangle/Shader.cpp
	
	src/first_triangle/base.vertexshader
	src/first_triangle/base.fragmentshader
)

# MVP Matrices
set(SRC_MVP_MATRICEX
	src/mvp_matrices/main.cpp
	src/mvp_matrices/Shader.h
	src/mvp_matrices/Shader.cpp

	src/mvp_matrices/mvp.vertexshader
	src/mvp_matrices/mvp.fragmentshader
)

# Cube
set(SRC_CUBE
	src/cube/main.cpp
	src/cube/Shader.h
	src/cube/Shader.cpp

	src/cube/cube.vertexshader
	src/cube/cube.vertexshader
)

# Cube Move
set(SRC_CUBE_MOVE
	src/cube_move/main.cpp
	src/cube_move/Shader.h
	src/cube_move/Shader.cpp

	src/cube_move/cube.vertexshader
	src/cube_move/cube.fragmentshader
)

# Motion Blur
set(SRC_MOTION_BLUR
	src/motion_blur/main.cpp
	src/motion_blur/Shader.h
	src/motion_blur/Shader.cpp

	src/cube_move/cube.vertexshader
	src/cube_move/cube.fragmentshader
	src/motion_blur/blur.vertexshader
	src/motion_blur/blur.fragmentshader
)

# Camera
set(SRC_Camera
	src/camera/main.cpp
	src/camera/Shader.h
	src/camera/Shader.cpp
	src/camera/Camera.h
	src/camera/Camera.cpp

	src/cube_move/cube.vertexshader
	src/cube_move/cube.fragmentshader
)

# Multibuffer
set(SRC_Multibuffer
	src/multibuffer/main.cpp
	src/multibuffer/Shader.h
	src/multibuffer/Shader.cpp

	src/multibuffer/multibuffer.vertexshader
	src/multibuffer/multibuffer.fragmentshader
)