
#include "renderer/renderer.h";

GLFWwindow *main_window;

int monitors;

int main() {
	init_GLFW();
	GLFWmonitor** m = glfwGetMonitors(&monitors);
	if (create_render_window(main_window, 1366, 768, "Render Window", m[1]) == -1)
		return -1;

	walls.push_back(GenWall(0.5f, 0.f, 0.f, -0.5f, 0.0f, 0.f, 0.f, 0.5f));

	render(main_window);
}