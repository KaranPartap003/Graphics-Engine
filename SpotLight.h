#pragma once
#include "PointLight.h"
#include <GLFW\glfw3.h>
class SpotLight :
    public PointLight
{
public:
    SpotLight();

    SpotLight(GLfloat shadowWidth, GLfloat shadowHeight,
              GLfloat near, GLfloat far,
              GLfloat red, GLfloat green, GLfloat blue,
              GLfloat aIntensity, GLfloat dIntensity,
              GLfloat xPos, GLfloat yPos, GLfloat zPos,
              GLfloat xDir, GLfloat yDir, GLfloat zDir,
              GLfloat con, GLfloat lin, GLfloat exp,
              GLfloat edg);

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
        GLuint edgeLocation);

    void SetFlash(glm::vec3 position, glm::vec3 direction, bool keys[]);

    ~SpotLight();

private:
    glm::vec3 direction;//light facing direction

    GLfloat edge, procEdge;
};

