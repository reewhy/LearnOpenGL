#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

float red = 0.3f;
float green = 0.2f;
float blue = 0.1f;

float trRed = 0.0f;
float trBlue = 0.0f;

int mode = GL_FILL;

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

	if (red > 1.1f) red = 0.0f;
	if (blue > 1.1f) blue = 0.0f;
	if (green > 1.1f) green = 0.0f;
}

void changeMode() {
	if (mode == GL_FILL) mode = GL_LINE;
	else mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void process_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_SPACE) glfwSetWindowShouldClose(window, true);
		if (key == GLFW_KEY_1) colorChange('r');
		if (key == GLFW_KEY_2) colorChange('g');
		if (key == GLFW_KEY_3) colorChange('b');
		if (key == GLFW_KEY_4) trRed = fmod(trRed + 0.1f, 1.1f);
		if (key == GLFW_KEY_5) trBlue = fmod(trBlue + 0.1f, 1.1f);
		if (key == GLFW_KEY_ENTER) changeMode();

		std::cout << "Red: " << red << " Blue: " << blue << " Green: " << green << std::endl;
	}

}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";


int main() {
	int success;
	char infoLog[512];

	glfwInit();
	// Window configuration options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
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
	// Setting window resize callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Setting input handling
	glfwSetKeyCallback(window, process_input_callback);
	
	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Creating a shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach the shader source code to the object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the shader
	glCompileShader(vertexShader);
	// Check success
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Linking and compiling fragment shader (this will give a color to our triangle)
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Create a shader program
	// (a shader program helps us combine multiple shaders)
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// Linking the shaders to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Delete shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Check success
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Vertex array
	float vertices[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f // top left
	};

	
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};


	// Creating a VAO (Vertex Arrat Objects buffer)
	unsigned int VAO;
	// Buffer of object
	unsigned int VBO;
	// Element Buffer Objects
	unsigned int EBO;
	glGenVertexArrays(1, &VAO);
	// Giving an ID to the buffer
	glGenBuffers(1, &VBO);
	// Giving an ID to the EBO
	glGenBuffers(1, &EBO);
	// Bind the vertex array
	glBindVertexArray(VAO);
	// Binding the buffe
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copying the verteces in the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Bind the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Telling OpenGL how to interpret vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Setting size of the rendering window
	// (with this function OpenGL knows that we want to display data and coordinates with respect to the window)
	glViewport(0, 0, 800, 600);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		// Rendering
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Change green value overtime
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		// Draw the object:
		//	Activate program
		glUseProgram(shaderProgram);
		//	Set uniform
		glUniform4f(vertexColorLocation, trRed, greenValue, trBlue, 1.0f);

		//  Bind the VAO
		glBindVertexArray(VAO);



		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//  DRAW THE FUCKING TRIANGLE(s)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Check and call  events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Clean GLFW's resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}