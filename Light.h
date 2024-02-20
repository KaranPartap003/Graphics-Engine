#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "ShadowMap.h"
class Light
{
public:
	Light();
	Light(GLfloat shadowWidth, GLfloat shadowHeight,
		  GLfloat red, GLfloat green, GLfloat blue, 
		  GLfloat aIntensity, GLfloat dIntensity);

	//for light on/off
	void SetDiffuseIntensity(GLfloat temp);
	GLfloat GetDiffuseIntensity();

	ShadowMap* GetShadowMap() { return shadowMap; }

	~Light();
protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	ShadowMap* shadowMap;
	glm::mat4 lightProj;
};

