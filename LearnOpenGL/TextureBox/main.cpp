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
const unsigned int SCR_WIDTH = 1100;
const unsigned int SCR_HEIGHT = 800;

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

// Background
glm::vec4 backColor(0.1f, 0.1f, 0.1f, 1.0f);

// Directional light
glm::vec3 direction(-0.2f, -1.0f, -0.3f);
glm::vec3 dirAmbient(0.05f, 0.05f, 0.05f);
glm::vec3 dirDiffuse(0.4f, 0.4f, 0.4f);
glm::vec3 dirSpecular(0.5f, 0.5f, 0.5f);
// Point light
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 pointSpec(0.5f, 0.5f, 0.5f);
glm::vec3 pointAmbient(0.5f, 0.5f, 0.5f);
glm::vec3 pointDiffuse(0.4f, 0.4f, 0.4f);
glm::vec3 pointColor(1.0f, 1.0f, 1.0f);
// Material
int shiny = 32;

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
	Shader lightingShader("../shader.vs", "../shader.fs");
	Shader lightCubeShader("../lightSource.vert", "../lightSource.frag");

	// Vertex array
	float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

	// Buffer of object
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	// Giving an ID to the buffer
	glGenBuffers(1, &VBO);
	// Bind the vertex array
	glBindVertexArray(cubeVAO);
	// Binding the buffe
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copying the verteces in the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Telling OpenGL how to interpret vertex data
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Lighting VAO
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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

	Texture texture("../textures/container2.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, true);
	Texture specularMap("../textures/container2_specular.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, true);

	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	lightingShader.setFloat("light.constant", 1.0f);
	lightingShader.setFloat("light.linear", 0.09f);
	lightingShader.setFloat("light.quadratic", 0.032f);


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

		glClearColor(backColor.x, backColor.y, backColor.z, backColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	Activate program
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", shiny);

		// Directional light
		lightingShader.setVec3("dirLight.direction", direction);
		lightingShader.setVec3("dirLight.ambient", dirAmbient);
		lightingShader.setVec3("dirLight.diffuse", dirDiffuse);
		lightingShader.setVec3("dirLight.specular",dirSpecular);
		// Point Light
		lightingShader.setVec3("pointLights[0].position", lightPos);
		lightingShader.setVec3("pointLights[0].ambient", pointAmbient);
		lightingShader.setVec3("pointLights[0].diffuse", pointDiffuse);
		lightingShader.setVec3("pointLights[0].specular", pointDiffuse);
		lightingShader.setVec3("pointLights[0].color", pointColor);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09f);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		lightingShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("view", view);
		// Set the shader projection
		lightingShader.setMat4("projection", projection);

		texture.use(GL_TEXTURE0);
		specularMap.use(GL_TEXTURE1);


		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

        glm::mat4 model = glm::mat4(1.0f);


		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightCubeShader.setMat4("model", model);
		lightCubeShader.setVec3("lightColor", pointColor);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Render the GUI
		ImGui::Render();
		// Render the data
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	// Clean GLFW's resources
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
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
	ImGui::ColorEdit4("Background color", glm::value_ptr((backColor)));

	if (!ImGui::CollapsingHeader("Directional Light")) {
		ImGui::Text("Directional light");
		ImGui::DragFloat3("dDirection", glm::value_ptr(direction), 0.1f);
		ImGui::DragFloat3("dAmbient", glm::value_ptr(dirAmbient), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat3("dDiffuse", glm::value_ptr(dirDiffuse), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat3("dSpecular", glm::value_ptr(dirSpecular), 0.01f, 0.0f, 1.0f);
	}
	if (!ImGui::CollapsingHeader("PointLight")) {
		ImGui::Text("Point Light");
		ImGui::DragFloat3("pPosition", glm::value_ptr(lightPos), 0.01f);
		ImGui::DragFloat3("pAmbient", glm::value_ptr(pointAmbient), 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat3("pDiffuse", glm::value_ptr(pointDiffuse), 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat3("pSpecular", glm::value_ptr(pointSpec), 0.1f, 0.0f, 1.0f);
		ImGui::ColorEdit3("pColor", glm::value_ptr(pointColor), ImGuiColorEditFlags_Float);
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