#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

#include "Texture.h"

// Window size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float offset = 0.0f;

// Rendering mode
int mode = GL_FILL;

// Callback for handling window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Change rendering mode
void changeMode();
// Process user input
void process_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
	// Initilize
	glfwInit();
	// Window configuration options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Creating a window object
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	// Checking if the object was created
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Making the window the main context of the current thread
	glfwMakeContextCurrent(window);
	// Setting window resize callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Setting input handling
	glfwSetKeyCallback(window, process_input_callback);
	
	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// TEST SHADER
	// Shader ourShader("../vertexTest.vs", "../fragmentTest.fs");
	Shader ourShader("../shader.vs", "../shader.fs");
	// Vertex array
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
   };

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// Creating a VAO (Vertex Arrat Objects buffer)
	unsigned int VAO;
	// Buffer of object
	unsigned int VBO;
	unsigned int EBO;
	glGenVertexArrays(1, &VAO);
	// Giving an ID to the buffer
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind the vertex array
	glBindVertexArray(VAO);
	// Binding the buffe
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copying the verteces in the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Telling OpenGL how to interpret vertex data
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	// test attribute
	// glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
	// glEnableVertexAttribArray(2);
	// texture coordinates attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// Setting size of the rendering window
	// (with this function OpenGL knows that we want to display data and coordinates with respect to the window)
	glViewport(0, 0, 800, 600);

	// Create the textures object

	Texture texture1("../textures/container.jpg", GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, true, GL_RGB);
	Texture texture2("../textures/awesomeface.png", GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, true, GL_RGBA);

	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    ourShader.setInt("texture2", 1);

	ourShader.setFloat("mesh", 0.2f);


	// Render loop
	while (!glfwWindowShouldClose(window)) {
		// Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		texture1.use(GL_TEXTURE0);
		texture2.use(GL_TEXTURE1);
		//	Activate program
		ourShader.use();
		glBindVertexArray(VAO);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//  DRAW THE FUCKING TRIANGLE(s)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Check and call  events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Clean GLFW's resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void changeMode() {
	if (mode == GL_FILL) mode = GL_LINE;
	else mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void process_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_SPACE) glfwSetWindowShouldClose(window, true);
		if (key == GLFW_KEY_ENTER) changeMode();
		if (key == GLFW_KEY_A) offset = offset - 0.1f;
		if (key == GLFW_KEY_D) offset = offset + 0.1f;
	}

}
