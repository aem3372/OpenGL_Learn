// Include standard headers
#include <stdio.h> 
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
// Pre-Builder define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Shader.h"

GLuint vertexbuffer, colorbuffer;
GLuint programID;
GLuint framebuffer;
GLuint renderbuffers[3];
GLuint depthbuffer;
static const GLenum fboBuffs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void sizeChangedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

bool checkErrors(GLuint progName)
{
	bool bFoundError = false;
	GLenum error = glGetError();

	if (error != GL_NO_ERROR)
	{
		fprintf(stderr, "A GL Error has occured\n");
		bFoundError = true;
	}

	GLenum fboStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		bFoundError = true;
		fprintf(stderr, "The framebuffer is not complete - ");
		switch (fboStatus)
		{
		case GL_FRAMEBUFFER_UNDEFINED:
			// Oops, no window exists?
			fprintf(stderr, "GL_FRAMEBUFFER_UNDEFINED\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			// Check the status of each attachment
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			// Attach at least one buffer to the FBO
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			// Check that all attachments enabled via
			// glDrawBuffers exist in FBO
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			// Check that the buffer specified via
			// glReadBuffer exists in FBO
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			// Reconsider formats used for attached buffers
			fprintf(stderr, "GL_FRAMEBUFFER_UNSUPPORTED\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			// Make sure the number of samples for each 
			// attachment is the same 
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			// Make sure the number of layers for each 
			// attachment is the same 
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n");
			break;
		}
	}

	if (progName != 0)
	{
		glValidateProgram(progName);
		int iIsProgValid = 0;
		glGetProgramiv(progName, GL_VALIDATE_STATUS, &iIsProgValid);
		if (iIsProgValid == 0)
		{
			bFoundError = true;
			fprintf(stderr, "The current program(%d) is not valid\n", progName);
		}
	}
	return bFoundError;
}

void renderInit(GLFWwindow* window) {
// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = { 
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	programID = loadShaders("multibuffer.vertexshader", "multibuffer.fragmentshader");

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	// Create framebuffer
	glGenFramebuffers(1, &framebuffer);

	// Create depth renderbuffer
	glGenRenderbuffers(1, &depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	// Create 3 color rednerbuffers
	glGenRenderbuffers(3, renderbuffers);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers[1]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers[2]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);

	// Attach all renderbuffers to FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffers[0]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, renderbuffers[1]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_RENDERBUFFER, renderbuffers[2]);

	//GLenum fboStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	if (checkErrors(0))
		exit(10);
}

void render(GLFWwindow* window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, width, height);
	glDrawBuffers(3, fboBuffs);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Draw the triangle !

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);

	// Projection matrix : 45бу Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around


	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);


	glDisable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);

	// Copy greyscale output to the left half of the screen
	glReadBuffer(GL_COLOR_ATTACHMENT2);

	glBlitFramebuffer(0, 0, width/2, height,
		              0, 0, width/2, height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
	// Copy the luminance adjusted color to the right half of the screen
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(width / 2, 0, width, height,
		              width / 2, 0, width, height,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	// Scale the unaltered image to the upper right of the screen
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, width, width,
		width - (int)(height*(0.2)), (int)(height*(0.8)),
		width, height,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void destoryCallback(GLFWwindow* window) {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteFramebuffers(1, &framebuffer);
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
