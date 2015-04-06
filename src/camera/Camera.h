// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
// Pre-Builder define GLEW_STATIC
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>


void initCamera();


// left and right
void strafe(float units);

// up and down
void fly(float units);

// foward and backward
void walk(float units);

// rotate on right vector
void pitch(float angle);

// rotate on up vector
void yaw(float angle);

// rotate on look vector
void roll(float angle);

glm::mat4 getCameraViewMatrix();
