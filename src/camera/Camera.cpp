#include "Camera.h"

static glm::vec3 up;
static glm::vec3 forward;
static glm::vec3 location;

void initCamera()
{
	location = glm::vec3(4.0f, 0.0f, 0.0f);
	forward = glm::vec3(-1.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

// left and right
void strafe(float units)
{
	location += glm::vec3(0, units, 0);
}

// up and down
void fly(float units)
{
	location += glm::vec3(0, 0, units);
}

// foward and backward
void walk(float units)
{
	location += glm::vec3(units, 0, 0);
}

// rotate on right vector
void pitch(float angle)
{

}

// rotate on up vector
void yaw(float angle)
{

}

// rotate on look vector
void roll(float angle)
{

}

glm::mat4 getCameraViewMatrix()
{
	up = glm::normalize(up);
	forward = glm::normalize(forward);
	glm::vec3 right = glm::cross(up,forward);

	glm::mat4 t(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
			   	0.0f, 0.0f, 1.0f, 0.0f,
				location[0], location[1], location[2], 1.0f);
	
	glm::mat4 r;
	r[0][0] = right[0]; r[0][1] = up[0]; r[0][2] = forward[0]; r[0][3] = 0.0f;
	r[1][0] = right[1]; r[1][1] = up[1]; r[1][2] = forward[1]; r[1][3] = 0.0f;
	r[2][0] = right[2]; r[2][1] = up[2]; r[2][2] = forward[2]; r[2][3] = 0.0f;
	r[3][0] = 0.0f;     r[3][1] = 0.0f;  r[3][2] = 0.0f;       r[3][3] = 1.0f;

	return t*r;
}
