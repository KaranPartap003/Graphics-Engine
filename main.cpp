// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "CommonValues.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "Model.h"
#include <assimp\Importer.hpp>
#include "Skybox.h"

#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_glfw.h"
#include "IMGUI\imgui_impl_opengl3.h"

const float toRadians = 3.14159265f / 180.0f;
GLfloat blackHawkAngle = 0.0f;

GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0,
	   uniformEyePos = 0, uniformSpecularIntensity = 0, uniformShininess = 0, 
	   uniformOmniLightPos = 0, uniformFarPlane = 0;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;
Material shinyMaterial;
Material dullMaterial;

Texture prism1;
Texture prism2;
Texture plainTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Model xWing;
Model blackHawk;
Model temple;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Skybox skybox;

//---------------------UI VARIABLES----------------------//  
std::vector<float> rotations{ 0.5f * toRadians };
glm::vec3 DirecColor = { 1.0f, 1.0f, 1.0f };
glm::vec3 PointColor = { 1.0f, 1.0f, 1.0f };
bool rotateDlight = false;
bool rotatePlight = false;
GLfloat Ddiffuse = 0.5f;
GLfloat Pdiffuse = 2.0f;
//---------------------UI VARIABLES----------------------//


unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

// Vertex Shader code
static const char* vShader = "Shaders/Shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/Shader.frag";


void calcAvgNormal(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int vertexCount, 
				  unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i+=3)
	{
		unsigned int i0 = indices[i] * vLength;
		unsigned int i1 = indices[i+1] * vLength;
		unsigned int i2 = indices[i+2] * vLength;
		glm::vec3 v1(vertices[i1] - vertices[i0], vertices[i1 + 1] - vertices[i0 + 1], vertices[i1 + 2] - vertices[i0 + 2]);
		glm::vec3 v2(vertices[i2] - vertices[i0], vertices[i2 + 1] - vertices[i0 + 1], vertices[i2 + 2] - vertices[i0 + 2]);
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		i0 += normalOffset, i1 += normalOffset, i2 += normalOffset;
		vertices[i0] += normal.x; vertices[i0 + 1] += normal.y; vertices[i0 + 2] += normal.z;
		vertices[i1] += normal.x; vertices[i1 + 1] += normal.y; vertices[i1 + 2] += normal.z;
		vertices[i2] += normal.x; vertices[i2 + 1] += normal.y; vertices[i2 + 2] += normal.z;
	}

	for (size_t i = 0; i < vertexCount / vLength; i++)//traversing each row of vertices array
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//   x		y	   z		u    v		nx	  ny	nz
		-1.0f, -1.0f, 0.0f,	  0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,	  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,	  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,	  0.5f, 1.0f,   0.0f, 0.0f, 0.0f
	};
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,  0.0f, 0.0f,	 0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,   10.0f, 0.0f,	 0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,   0.0f, 10.0f,	 0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,    10.0f, 10.0f, 0.0f, -1.0f, 0.0f
	};

	calcAvgNormal(indices, 12, vertices, 32, 8, 5);

	//Mesh* obj1 = new Mesh();
	//obj1->CreateMesh(vertices, indices, 32, 12);
	//meshList.push_back(obj1);
	//
	//Mesh* obj2 = new Mesh();
	//obj2->CreateMesh(vertices, indices, 32, 12);
	//meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShader()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFile(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFile("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");

	omniShadowShader = Shader();
	omniShadowShader.CreateFromFile("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");

}

void RenderScene()
{
	glm::mat4 model(1.0f);

	//for surface :
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -4.0f, -8.5f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	plainTexture.UseTexture();
	meshList[0]->RenderMesh();

	//for temple :
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -4.0f, -5.5f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	temple.RenderModel();

}

void DirectionalShadowMapPass(DirectionalLight *light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	glm::mat4 lightTransform = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&lightTransform);
	directionalShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();
	glm::vec3 lightPos = light->GetPosition();
	glUniform3f(uniformOmniLightPos, lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());
	omniShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	glViewport(0, 0, 1366, 768);

	// Clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//imgui code
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	skybox.Draw(viewMatrix, projectionMatrix);
	
	shaderList[0].UseShader();
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePos = shaderList[0].GetuniformEyePosition();
	uniformSpecularIntensity = shaderList[0].GetuniformSpecularIntensity();
	uniformShininess = shaderList[0].GetShininessLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePos, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

	shaderList[0].SetDirectionalLight(&mainLight, 0.5f*toRadians, DirecColor, Ddiffuse, rotateDlight); //
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0, rotations, PointColor, Pdiffuse,rotatePlight);
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	glm::mat4 LightTransform = mainLight.CalculateLightTransform();
	shaderList[0].SetDirectionalLightTransform(&LightTransform);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);
	
	shaderList[0].Validate();
	glm::vec3 lowerLight = camera.GetCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera.GetCameraDirection(), mainWindow.getKeys());
	RenderScene();
}

int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.initialise();
	CreateObjects();
	CreateShader();

	camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);
	mainLight = DirectionalLight(2048, 2048,
					0.8f, 0.8f, 1.0f,
					0.0f, 0.5f, 
					10.0f, -5.0f, 1.0f);
	
	pointLights[0] = PointLight(1024, 1024,
				    0.1, 100,
				    1.0f, 1.0f, 1.0f,
				    0.2f, 2.0f,
				    -2.0f, 0.0f, -3.0f,
				    0.3f, 0.1f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(1024, 1024,
					0.1, 100,
					1.0f, 1.0f, 1.0f,
					0.2f, 2.0f,
					-4.0f, 0.0f, 0.0f,
					0.3f, 0.1f, 0.1f);
	//pointLightCount++;
	
	spotLights[0] = SpotLight(1024, 1024,
				  0.1, 100, 
				  1.0f, 1.0f, 1.0f,
				  0.0f, 0.0f,
				  4.0f, 0.0f, 0.0f,
				  0.0f, -1.0f, 0.0f,
				  0.3f, 0.1f, 0.1f, 
				  20.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(1024, 1024,
				  0.1, 100, 
				  1.0f, 1.0f, 1.0f,
				  0.0f, 0.0f,
				  0.0f, 2.0f, -4.0f,
				  0.0f, -1.0f, 0.0f,
				  0.3f, 0.1f, 0.1f,
				  20.0f);
	spotLightCount++;

	std::vector<std::string> skyboxFaces;
	/*
	skyboxes:
		bloody-heresy
		nightsky
		badomen
	*/
	skyboxFaces.push_back("textures/skybox/nightsky_rt.tga");
	skyboxFaces.push_back("textures/skybox/nightsky_lf.tga");
	skyboxFaces.push_back("textures/skybox/nightsky_up.tga");
	skyboxFaces.push_back("textures/skybox/nightsky_dn.tga");
	skyboxFaces.push_back("textures/skybox/nightsky_bk.tga");
	skyboxFaces.push_back("textures/skybox/nightsky_ft.tga");

	skybox = Skybox(skyboxFaces);

	plainTexture = Texture("textures/plain.png");
	plainTexture.LoadTextureA();

	temple = Model();
	temple.LoadModel("../models/Japanese_Temple.obj");

	shinyMaterial = Material(5.0f, 32.0f);
	dullMaterial = Material(0.3f, 4.0f);

	Assimp::Importer importer;

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);
	
	//imgui initialisation
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(mainWindow.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		// Get + Handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		GLenum Q = glfwGetKey(mainWindow.getWindow(), GLFW_KEY_Q);
		if (Q == GLFW_RELEASE)
		{
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange(), deltaTime);
			glfwSetInputMode(mainWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);//DISABLED, NORMAL, HIDDEN
		}
		else {
			glfwSetInputMode(mainWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);//DISABLED, NORMAL, HIDDEN
		}

		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}
		RenderPass(projection, camera.calculateViewMatrix());
		ImGui::Begin("hold Q to access UI elements");
		//add imgui code here:
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		if (ImGui::Button("toggle rotation of directional light"))
			rotateDlight = !rotateDlight;
		if (ImGui::Button("toggle rotation of point light"))
			rotatePlight = !rotatePlight;
		ImGui::ColorEdit3("Directional light color ", (float*)&DirecColor);
		ImGui::ColorEdit3("Point light color ", (float*)&PointColor);
		ImGui::SliderFloat("Directional diffuse", &Ddiffuse, 0.0f, 5.0f);
		ImGui::SliderFloat("Point diffuse", &Pdiffuse, 0.0f, 5.0f);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glUseProgram(0);
		mainWindow.swapBuffer();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	return 0;
}
