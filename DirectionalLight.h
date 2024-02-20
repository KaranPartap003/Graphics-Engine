#pragma once
#include "Light.h"
#include <glm\gtc\matrix_transform.hpp>
class DirectionalLight :
    public Light
{
public:
    DirectionalLight();
    ~DirectionalLight();
    DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
                     GLfloat red, GLfloat green, GLfloat blue, 
                     GLfloat aIntensity, GLfloat dIntensity,
                     GLfloat xDir, GLfloat yDir, GLfloat zDir);

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
        GLuint diffuseIntensityLocation, GLuint directionLocation);
    glm::vec3 GetDirection() { return direction; }
    void SetDirection(glm::vec3 dir);
    glm::mat4 CalculateLightTransform();

private:
    glm::vec3 direction;
};

