#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Callback for handling window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Close on "space" press
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) std::cout << "Pressed" << std::endl;
}

int main() {
	glfwInit();
	// Window configuration options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	
	// Creating a window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	// Checking if the object was created
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Making the window the main context of the current thread
	glfwMakeContextCurrent(window);
	
	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Setting size of the rendering window
	// (with this function OpenGL knows that we want to display data and coordinates with respect to the window)
	glViewport(0, 0, 800, 600);

	// Setting window resize callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		// Input
		processInput(window);

		// Rendering

		// Check and call  events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Clean GLFW's resources
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}