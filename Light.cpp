#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0;
}

Light::Light(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;

	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

void Light::SetDiffuseIntensity(GLfloat temp)
{
	diffuseIntensity = temp;
}

GLfloat Light::GetDiffuseIntensity()
{
	return diffuseIntensity;
}

Light::~Light()
{
}
