// Include standard headers
#include <stdio.h> 
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
// Pre-Builder define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"

GLuint VertextArrayID;
GLuint vertexbuffer;
GLuint programID;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void sizeChangedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void renderInit(GLFWwindow* window) {
	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	glGenVertexArrays(1, &VertextArrayID);
	glBindVertexArray(VertextArrayID);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	programID = loadShaders("base.vertexshader", "base.fragmentshader");
}

void render(GLFWwindow* window) {
	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);
	
	// Draw the triangle !

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);

	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);
}

void destoryCallback(GLFWwindow* window) {
	glDeleteBuffers(1, &vertexbuffer);
}

int main() {
	
	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFWn");
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	glfwMakeContextCurrent(window);

	// glewInit must be called after glfwMakeContextCurrent
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEWn");
		return -1;
	}

	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowSizeCallback(window, sizeChangedCallback);
	glfwSetWindowCloseCallback(window, destoryCallback);
	renderInit(window);

	while (!glfwWindowShouldClose(window))
	{
		render(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}
