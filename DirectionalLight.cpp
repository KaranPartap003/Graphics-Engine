#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightProj = glm::ortho( -20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

DirectionalLight::DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
								   GLfloat red, GLfloat green, GLfloat blue,
								   GLfloat aIntensity, GLfloat dIntensity,
								   GLfloat xDir, GLfloat yDir, GLfloat zDir):Light(shadowWidth, shadowHeight,
																				   red, green, blue, 
																				   aIntensity, dIntensity)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	lightProj = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, 0.1f, 100.0f);
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

void DirectionalLight::SetDirection(glm::vec3 dir)
{
	direction = dir;
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight()
{
}