// Helps make OpenGL compatible on more devices
#include <glad/glad.h>
// I forgor
#include <GLFW/glfw3.h>
#include <iostream>
// Class for easy shader loading
#include "../common/Shader.h"
#include <filesystem>
// Class for image loading
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
// Class for easy texture loading
#include "../common/Texture.h"

// math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../common/Camera.h"
#include "imgui.h"
#include "../libs/imgui/imgui_impl_glfw.h"
#include "../libs/imgui/imgui_impl_opengl3.h"
// Window size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Rendering mode
int mode = GL_FILL;
int camera_mode = GLFW_CURSOR_DISABLED;

// Top-down
GLFWwindow* init();
void drawCube(Shader ourShader, glm::vec3 position, float angle, bool rotate);
// Callback for handling window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Change rendering mode
void changeMode();
void changeCameraMode(GLFWwindow* window);
// Process user input
void process_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
// Gui
void initGui(GLFWwindow* window);
void gui(bool *wireframe);

// Camera settings
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f; // Time between current and last frame
float lastFrame = 0.0f;

int main() {
	bool wireframe = false;
	// Init window
	GLFWwindow* window = init();

	if (window == nullptr) return -1;
	// Init ImGui
	initGui(window);

	// Yes
	glEnable(GL_DEPTH_TEST);

	// Load shaders
	Shader ourShader("../shader.vs", "../shader.fs");
	// Vertex array
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Creating a VAO (Vertex Array Objects buffer)
	unsigned int VAO;
	// Buffer of object
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	// Giving an ID to the buffer
	glGenBuffers(1, &VBO);
	// Bind the vertex array
	glBindVertexArray(VAO);
	// Binding the buffe
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copying the verteces in the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Telling OpenGL how to interpret vertex data
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Create the textures object
	Texture texture1("../textures/container.jpg", GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, true, GL_RGB);
	Texture texture2("../textures/awesomeface.png", GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, true, GL_RGBA);

	// Positions of cubes in the space
	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Use the shader
	ourShader.use();
	ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
	ourShader.setFloat("mesh", 0.2f);

	// Light color
	glm::vec3 coral(1.0f, 0.5f, 0.31f);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Calculate deltaTime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Process user movement input
		processInput(window);

		// Generate ImGui frames
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Generate UI
		gui(&wireframe);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the texture
		texture1.use(GL_TEXTURE0);
		texture2.use(GL_TEXTURE1);

		//	Activate program
		ourShader.use();
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);
		// Set the shader projection
		ourShader.setMat4("projection", projection);

		glBindVertexArray(VAO);
		// Draw all the cubes
		for(unsigned int i = 0; i < 10; i++)
		{
			bool rotate = false;
			if (i % 3 == 0 || i == 0) rotate = true;
			float angle = 20.0f * i;
			drawCube(ourShader, cubePositions[i], angle, rotate);
		}

		glBindVertexArray(VAO);

		// Render the GUI
		ImGui::Render();
		// Render the data
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	// Clean GLFW's resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

// Function to draw cubes
void drawCube(Shader ourShader, glm::vec3 position, float angle, bool rotate) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	if ( rotate) {
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	}
	ourShader.setMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

// Initialize GUI
void initGui(GLFWwindow *window) {
	// ImGui configuration
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
	ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
	ImGui_ImplOpenGL3_Init("#version 330");
}

// Initialize GLFW
GLFWwindow* init() {
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
		return nullptr;
	}
	// Making the window the main context of the current thread
	glfwMakeContextCurrent(window);
	// Setting window resize callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Setting input handling
	glfwSetKeyCallback(window, process_input_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	return window;
}

// GUI Configs
void gui(bool *wireframe){
	ImGui::Begin("Settings");

	if (ImGui::Checkbox("Wireframe mode", wireframe)) {
		changeMode();
	}

	ImGui::End();
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
		// if (key == GLFW_KEY_ENTER) changeMode();
		if (key == GLFW_KEY_F) changeCameraMode(window);
	}

}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void changeCameraMode(GLFWwindow* window) {
	if (camera_mode == GLFW_CURSOR_DISABLED) camera_mode = GLFW_CURSOR_NORMAL;
	else camera_mode = GLFW_CURSOR_DISABLED;

	if (camera_mode == GLFW_CURSOR_NORMAL) {
		camera.MouseSensitivity = 0.0f;
	} else {
		camera.MouseSensitivity = 0.1f;
	}
	glfwSetInputMode(window, GLFW_CURSOR, camera_mode);
}