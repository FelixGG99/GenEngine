#pragma once
#ifndef GEN_ENG_RENDERER_H
#define GEN_ENG_RENDERER_H

// Renderer Header depends on:

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "level_editor/3dobj.h"
#include "renderer/view.h"
#include "renderer/shader.h"
#include "util/vec.h"
#include "util/camera.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>

std::vector<GenWall>walls;

// REMOVE
void framebuffer_callback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}
/// DELETE
void	drawFloorPlane(const Shader& shader, const mat4x4 view, const mat4x4 projection);
void	drawAxis(const Shader& shader);

int		init_OpenGL_APIs();
int		create_render_window(GLFWwindow *&p_window, const int w, const int h, const char *title, GLFWmonitor *monitor);
int		render();

void	setGradientColor(const vec3& top, const vec3& bot);


// Initialize the GLFW API.
int init_GLFW() {

	// Initialize GLFW and set window hints regarding the characteristics of the OpenGL version we're using.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// If everything was initialized successfully, return 1.
	return 1;
}

// Creates a new window given according to the user requirements and assigns it to a given GLFWwindow pointer.
int create_render_window(GLFWwindow *&p_window, const int w, const int h, const char* title, GLFWmonitor* monitor) {

	p_window = glfwCreateWindow(w, h, title, monitor, NULL);
	std::cout << p_window << "\n";
	if (!p_window) {
		std::cout << "Window \"" << title << "\" could not be created.\n";
		return -1;
	}
	glfwMakeContextCurrent(p_window);

	// Initialize GLAD. If it can't be started, terminate GLFW and return a value indicating the APIs weren't initialized.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD could not be initialized.\n";
		glfwTerminate();
		return -2;
	}
	glfwSetFramebufferSizeCallback(p_window, framebuffer_callback);

	glViewport(0, 0, w, h);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	return 1;
}

// Main render function
int render(GLFWwindow*& p_window) {
	static mat4x4 view, projection;
	static Shader plane("shaders/vs_proj.vs", "shaders/fs_col.fs");
	GenEngine::Camera camera(vec3(0.f, 0.f, 6.f), vec3(0.f, 180.f, 180.f));
	glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (true) {
		camera.move_camera(p_window);
		camera.cursor_offset_to_angle(p_window);
		camera.angles_to_axis();
		view = camera.look_at();
		projection = getProjMatrix(1366, 768, 0.01f, 10.f, 90.f);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		setGradientColor(vec3(0.f, 1.f, 0.f) * std::max(0.1f, sin((float)glfwGetTime())), vec3(0.f, 0.f, 1.f) * std::max(0.1f, cos((float)glfwGetTime())));

		for (auto i = walls.begin(); i != walls.end(); i++) {
			i->draw(plane, view, projection);
		}
		//drawFloorPlane(plane, view, projection);
		glfwSwapBuffers(p_window);
		glfwPollEvents();
	}
	return 1;
}

void setGradientColor(const vec3& top, const vec3& bot) {
	glDisable(GL_DEPTH_TEST);
	static unsigned int backgroundVAO = 0;
	static Shader background;

	if (!backgroundVAO) {
		glGenVertexArrays(1, &backgroundVAO);
		background.compile("shaders/vs_background_dg.vs", "shaders/fs_background_dg.fs");
	}
	background.use();
	background.setVec3f("topColor", top.x(), top.y(), top.z());
	background.setVec3f("botColor", bot.x(), bot.y(), bot.z());
	glBindVertexArray(backgroundVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

// DELETE THIS-------------------------------------!!!!!!!!!!!!!!
const vec3 axis[] = {
	///vertices				//colors
	{-50.0f, 0.0f, 0.0f},	{1.0f,0.0f,0.0f},	//eje X: rojo
	{50.0f, 0.0f, 0.0f},	{1.0f,0.0f,0.0f},

	{0.0f, 0.0f, 50.0f},	{0.0f,0.0f,1.0f}, //eje Z: azul
	{0.0f, 0.0f,-50.0f},	{0.0f,0.0f,1.0f},

	{0.0f, 100.0f, 0.0f},	{0.0f,1.0f,0.0f}, //eje Y: verde
	{0.0f,-100.0f,0.0f},	{0.0f,1.0f,0.0f}
};

const float square[] = {
	-1.0f, 0.0f, 1.0f,
	 1.0f, 0.0f, 1.0f,
	-1.0f, 0.0f,-1.0f,
	 1.0f, 0.0f,-1.0f
};

const vec3 xDivisions[] = {
	{0.0f, 1.0f, -0.1f},	{1.0f,0.0f,0.0f},
	{0.0f,-1.0f, -0.1f}, {1.0f,0.0f,0.0f}
};
const unsigned int iAxis[] = { 0,1,	2,3, 4,5 };
const unsigned int iX[] = { 0,1 };


void xMeasures(const Shader& shader) {

	static unsigned int xmVao = 0;
	static unsigned int xmVbo = 0;
	static unsigned int xmEbo = 0;
	static mat4x4 model;
	if (!xmVao) {
		glGenVertexArrays(1, &xmVao);
		glGenBuffers(1, &xmVbo);
		glGenBuffers(1, &xmEbo);
		glBindVertexArray(xmVao);
		glBindBuffer(GL_VERTEX_ARRAY, xmVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(xDivisions), &xDivisions[0].e[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, xmEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(iX), iX, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	shader.use();
	glBindVertexArray(xmVao);
	for (int i = 1; i < 2; i++) {
		model(3, 0) = ((float)i);
		shader.setMat4f("model", model);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
		model(3, 0) = ((float)(-i));
		shader.setMat4f("model", model);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	}

}

void drawFloorPlane(const Shader& shader, const mat4x4 view, const mat4x4 projection) {
	static unsigned int vao = 0;
	static unsigned int vbo = 0;
	if (!vao) {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	shader.use();
	shader.setMat4f("view", view);
	shader.setMat4f("projection", projection);
	shader.setVec3f("color", 0.8f, 0.8f, 0.8f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
}

// MOVE THIS FUNCTION OUT OF HERE NEXT TIME YOU EDIT THIS YOU LAZY PIECE OF SHIT
void drawAxis(const Shader& shader) {
	static unsigned int axisVAO = 0;
	static unsigned int axisVBO = 0;
	static unsigned int axisEBO = 0;
	static mat4x4 modelA;
	if (!axisVAO) {
		glGenVertexArrays(1, &axisVAO);
		glGenBuffers(1, &axisVBO);
		glGenBuffers(1, &axisEBO);
		glBindVertexArray(axisVAO);
		glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(axis), &axis[0].e[0], GL_DYNAMIC_DRAW);
		//buffer data: target, sizeof, data of array where info is, mode
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axisEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(iAxis), iAxis, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	shader.use();
	shader.setMat4f("model", modelA);
	glBindVertexArray(axisVAO);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
}

#endif // !GEN_ENG_RENDERER_H
