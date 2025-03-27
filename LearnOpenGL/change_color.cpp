#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

float red = 0.3f;
float green = 0.2f;
float blue = 0.1f;

// Callback for handling window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void colorChange(char c) {
	if (c == 'r') {
		red = red + 0.1f;
	}
	if (c == 'b') {
		blue = blue + 0.1f;
	}
	if (c == 'g') {
		green = green + 0.1f;
	}

	if (red > 1.0f) red = 0.0f;
	if (blue > 1.0f) blue = 0.0f;
	if (green > 1.0f) green = 0.0f;
}

void process_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_SPACE) glfwSetWindowShouldClose(window, true);
		if (key == GLFW_KEY_0) colorChange('r');
		if (key == GLFW_KEY_1) colorChange('g');
		if (key == GLFW_KEY_2) colorChange('b');

		std::cout << "Red: " << red << " Blue: " << blue << " Green: " << green << std::endl;
	}

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
	// Setting input handling
	glfwSetKeyCallback(window, process_input_callback);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		// Rendering
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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