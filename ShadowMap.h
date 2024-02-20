#pragma once
#include <stdio.h>
#include <GL\glew.h>

class ShadowMap
{
public:
	ShadowMap();

	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum textureUnit);
	GLuint GetShadowWidth() { return shadowWidth; }
	GLuint GetShadowHeight() { return shadowHeight; }

	~ShadowMap();
protected:
	GLuint FBO, shadowMap;
	GLuint shadowWidth, shadowHeight;
};

