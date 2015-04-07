#include "Camera.h"

static glm::vec3 up;
static glm::vec3 forward;
static glm::vec3 location;

void initCamera()
{
	location = glm::vec3(0.0f, 0.0f, 0.0f);
	forward = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

// left and right
void strafe(float units)
{
	glm::vec3 right = glm::cross(forward, up);
	location += right * units;
}

// up and down
void fly(float units)
{
    location += up * units;
}

// foward and backward
void walk(float units)
{
    location += forward * units;    
}

// rotate on right vector
void pitch(float angle)
{
	forward = glm::rotateX(forward, angle);
	up = glm::rotateX(up, angle);
}

// rotate on up vector
void yaw(float angle)
{
	forward = glm::rotateY(forward, angle);
}

// rotate on look vector
void roll(float angle)
{
	up = glm::rotateZ(up, angle);
}

glm::mat4 getCameraViewMatrix()
{
    forward = glm::normalize(forward);
    up = glm::normalize(up);
    glm::vec3 right = glm::cross(forward,up);
    
    glm::mat4 r;
    r[0][0] = right[0]; r[0][1] = up[0]; r[0][2] = -forward[0]; r[0][3] = 0.0f;
    r[1][0] = right[1]; r[1][1] = up[1]; r[1][2] = -forward[1]; r[1][3] = 0.0f;
    r[2][0] = right[2]; r[2][1] = up[2]; r[2][2] = -forward[2]; r[2][3] = 0.0f;
    
    r[3][0] = -glm::dot(location,right);
    r[3][1] = -glm::dot(location,up);
    r[3][2] = glm::dot(location,forward);
    r[3][3] = 1.0f;

    return r;
}
