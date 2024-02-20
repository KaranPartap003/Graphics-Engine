#pragma once
#include <GL\glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);
	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
	~Material();
private:
	GLfloat specularIntensity; //how much specular light should be on an object
	GLfloat shininess;		   //how collected the light is in a region
};

