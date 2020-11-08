#pragma once
#ifndef GEN_WINDOW_H
#define GEN_WINDOW_H

#include <GLFW/glfw3.h>

class GenWindow {
	/// CHECK https://stackoverflow.com/questions/7676971/pointing-to-a-function-that-is-a-class-member-glfw-setkeycallback
	GLFWwindow *w;

	int w, h;
	int x, y;
};

#endif // !GEN_WINDOW_H
