#ifndef SHADER_H
#define SHADER_H

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
// Pre-Builder define GLEW_STATIC
#include <GL/glew.h>

// Read the VertexShader and FragmentShader code from the file.
// Next, create a simple shader program(Compile, Link, Check)
// @return ProgramID
GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);

#endif //SHADER_H