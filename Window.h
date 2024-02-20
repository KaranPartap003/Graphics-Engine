#pragma once

#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>


class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint WindowHeight);

	int initialise();

	GLfloat GetBufferWidth() { return bufferWidth; }
	GLfloat GetBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLFWwindow* getWindow() { return mainWindow; }

	void swapBuffer() { glfwSwapBuffers(mainWindow); }
	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX, lastY, xChange, yChange;
	bool mouseFirstMoved;

	void createCallBacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

