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

GLuint blurTextures[16];
GLuint vertexbuffer, colorbuffer, pixelbuffer;
GLuint texvertexbuffer, texcoordbuffer;
GLuint programID;
GLuint blurprogramID;
GLuint curBlurTarget;
void *pixelData;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void sizeChangedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


// returns 1 - 6 for blur texture units
// curPixBuf is always between 0 and 5
void AdvanceBlurTaget() { curBlurTarget = ((curBlurTarget + 1) % 16); }
GLuint GetBlurTarget0(){ return (1 + ((curBlurTarget + 15) % 16)); }
GLuint GetBlurTarget1(){ return (1 + ((curBlurTarget + 14) % 16)); }
GLuint GetBlurTarget2(){ return (1 + ((curBlurTarget + 13) % 16)); }
GLuint GetBlurTarget3(){ return (1 + ((curBlurTarget + 12) % 16)); }
GLuint GetBlurTarget4(){ return (1 + ((curBlurTarget + 11) % 16)); }
GLuint GetBlurTarget5(){ return (1 + ((curBlurTarget + 10) % 16)); }
GLuint GetBlurTarget6(){ return (1 + ((curBlurTarget + 9) % 16)); }
GLuint GetBlurTarget7(){ return (1 + ((curBlurTarget + 8) % 16)); }
GLuint GetBlurTarget8(){ return (1 + ((curBlurTarget + 7) % 16)); }
GLuint GetBlurTarget9(){ return (1 + ((curBlurTarget + 6) % 16)); }
GLuint GetBlurTarget10(){ return (1 + ((curBlurTarget + 5) % 16)); }
GLuint GetBlurTarget11(){ return (1 + ((curBlurTarget + 4) % 16)); }
GLuint GetBlurTarget12(){ return (1 + ((curBlurTarget + 3) % 16)); }
GLuint GetBlurTarget13(){ return (1 + ((curBlurTarget + 2) % 16)); }
GLuint GetBlurTarget14(){ return (1 + ((curBlurTarget + 1) % 16)); }
GLuint GetBlurTarget15(){ return (1 + ((curBlurTarget) % 16)); }

void renderInit(GLFWwindow* window) {
	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,
		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,
		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,
		0.014f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,
		0.676f, 0.977f, 0.133f,
		0.971f, 0.572f, 0.833f,
		0.140f, 0.616f, 0.489f,
		0.997f, 0.513f, 0.064f,
		0.945f, 0.719f, 0.592f,
		0.543f, 0.021f, 0.978f,
		0.279f, 0.317f, 0.505f,
		0.167f, 0.620f, 0.077f,
		0.347f, 0.857f, 0.137f,
		0.055f, 0.953f, 0.042f,
		0.714f, 0.505f, 0.345f,
		0.783f, 0.290f, 0.734f,
		0.722f, 0.645f, 0.174f,
		0.302f, 0.455f, 0.848f,
		0.225f, 0.587f, 0.040f,
		0.517f, 0.713f, 0.338f,
		0.053f, 0.959f, 0.120f,
		0.393f, 0.621f, 0.362f,
		0.673f, 0.211f, 0.457f,
		0.820f, 0.883f, 0.371f,
		0.982f, 0.099f, 0.879f
	};
	static const GLfloat g_texvectex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};
	static const GLfloat g_texcoord_buffer_data[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_MULTISAMPLE);

	programID = loadShaders("cube.vertexshader", "cube.fragmentshader");

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &texvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_texvectex_buffer_data), g_texvectex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &texcoordbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_texcoord_buffer_data), g_texcoord_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	blurprogramID = loadShaders("blur.vertexshader", "blur.fragmentshader");
	glGenTextures(6, blurTextures);
	
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	unsigned int pixelDataSize = width * height * 3 * sizeof(unsigned int); // XXX This should be unsigned byte

	// Initialize texture data
	for (int i = 0; i<16; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, blurTextures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}

	// Alloc space for copying pixels so we dont call malloc on every draw
	glGenBuffers(1, &pixelbuffer);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelbuffer);
	glBufferData(GL_PIXEL_PACK_BUFFER, pixelDataSize, pixelData, GL_DYNAMIC_COPY);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

}

void render(GLFWwindow* window, double time) {
	static float tx = 0.0f;
	static float dx = 2500.0f;
	if (tx + dx*time > 3.5)
	{
		dx = -2500;
		tx = 3.5 - (tx + dx*time - 3.5);
	}
	if (tx + dx*time < -3.5)
	{
		dx = 2500;
		tx = -3.5 + (tx + dx*time + 3.5);
	}
	tx += time * dx;
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

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::translate(tx, 0.0f, 0.0f);  // Changes for each model !
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // Starting from vertex 0; 3 vertices total -> 1 triangle


	
	int width, height;
	glfwGetWindowSize(window, &width, &height); 


	glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelbuffer);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelbuffer);
	glActiveTexture(GL_TEXTURE0 + GetBlurTarget0());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	


	glDisable(GL_DEPTH_TEST);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, texvertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordbuffer);
	glVertexAttribPointer(
		1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glUseProgram(blurprogramID);
	MVP = glm::mat4();
	glUniformMatrix4fv(glGetUniformLocation(blurprogramID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	// Setup the textue units for the blur targets, these rotate every frame
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit0"), GetBlurTarget0());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit1"), GetBlurTarget1());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit2"), GetBlurTarget2());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit3"), GetBlurTarget3());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit4"), GetBlurTarget4());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit5"), GetBlurTarget5());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit6"), GetBlurTarget6());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit7"), GetBlurTarget7());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit8"), GetBlurTarget8());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit9"), GetBlurTarget9());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit10"), GetBlurTarget10());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit11"), GetBlurTarget11());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit12"), GetBlurTarget12());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit13"), GetBlurTarget13());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit14"), GetBlurTarget14());
	glUniform1i(glGetUniformLocation(blurprogramID, "textureUnit15"), GetBlurTarget15());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	AdvanceBlurTaget();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void destoryCallback(GLFWwindow* window) {
	glDeleteBuffers(1, &vertexbuffer);
}

void updateFps()
{
	static int fc = 0;
	static double time = glfwGetTime();
	fc++;
	if (fc > 500)
	{
		double now = glfwGetTime();
		printf("FPS: %.2f\n", 500 / (now - time));
		time = now;
		fc = 0;
	}
}

int main() {

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFWn");
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "My Title", NULL, NULL);
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

	double lasttime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		double nowtime = glfwGetTime();
		render(window, (nowtime - lasttime)/100);
		lasttime = nowtime;

		updateFps();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

// 帧率过高时效果不明显