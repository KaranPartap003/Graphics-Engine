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
	
	ShadowMap* GetShadowMap() { return shadowMap; }
	
	//for UI 
	void SetDiffuseIntensity(GLfloat temp);
	void SetColor(glm::vec3 myColor) { color = myColor; }
	~Light();
protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	ShadowMap* shadowMap;
	glm::mat4 lightProj;
};

