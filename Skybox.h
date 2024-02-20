#pragma once
#include "CommonValues.h"
#include <vector>
#include <string>

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Mesh.h"

class Skybox
{
public:
	Skybox();

	Skybox(std::vector<std::string> faceLocations);
	void Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	~Skybox();

private:
	Mesh* skyMesh;
	Shader* skyShader;

	GLuint textureID;
	GLuint uniformProjection, uniformView;
};

