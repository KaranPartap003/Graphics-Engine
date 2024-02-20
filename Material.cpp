#include "Material.h"

Material::Material()
{
	shininess = 0.0f;
	specularIntensity = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	shininess = shine;
	specularIntensity = sIntensity;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}

Material::~Material()
{

}
