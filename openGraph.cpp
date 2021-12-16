#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"

constexpr int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;


void initialize_glfw();
void initialize_glad();
GLFWwindow* create_window();
void center_window(GLFWwindow*);
void set_callbacks(GLFWwindow*);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keypress_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
	initialize_glfw();

	GLFWwindow* window = create_window();
	glfwMakeContextCurrent(window);
	center_window(window);
	glfwShowWindow(window);
	initialize_glad();

	ShaderProgram bottomShader({ {"res/bottomFragmentShader.frag", GL_FRAGMENT_SHADER}, { "res/vertexShader.vert", GL_VERTEX_SHADER } });
	ShaderProgram topShader({ {"res/topFragmentShader.frag", GL_FRAGMENT_SHADER}, { "res/vertexShader.vert", GL_VERTEX_SHADER } });

	set_callbacks(window);
	float bottomTriangles[] = {
		-0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.2f, 0.2f, 0.0f, 0.0f, 0.0f, 1.0f,

		0.3f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.1f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.2f, 0.2f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	float topTriangle[] = {
		-0.1f, 0.3f, 0.0f,
		0.1f, 0.3f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2,   // first triangle
		3, 4, 5    // second triangle
	};

	// Generate a buffer to store the element information (indices).
	unsigned int elementBufferObject;
	glGenBuffers(1, &elementBufferObject);

	// Generate the VertexArrayObjects and VertexBufferObjects
	unsigned int bottomTrianglesVAO, bottomVBO, topTriangleVAO, topVBO;
	glGenVertexArrays(1, &bottomTrianglesVAO);
	glGenVertexArrays(1, &topTriangleVAO);
	glGenBuffers(1, &bottomVBO);
	glGenBuffers(1, &topVBO);

	// Bind bottom VAO
	glBindVertexArray(bottomTrianglesVAO);
	
	// Load the data in bottomTriangles into bottom VBO
	glBindBuffer(GL_ARRAY_BUFFER, bottomVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bottomTriangles), bottomTriangles, GL_STATIC_DRAW);

	// Load the data in indices into elementBufferObject
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(topTriangleVAO);

	glBindBuffer(GL_ARRAY_BUFFER, topVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(topTriangle), topTriangle, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	topShader.use();

	topShader.setFloat("offset", -0.3f, 0.0f);

	bottomShader.use();
	bottomShader.setFloat("offset", -0.3f, 0.1f);


	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		bottomShader.use();

		glBindVertexArray(bottomTrianglesVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		topShader.use();

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		topShader.setFloat("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
		
		glBindVertexArray(topTriangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}	

GLFWwindow* create_window() {
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PLZ WINDOW??", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed creating window" << std::endl;
		std::cout << glfwGetError(nullptr) << '\n';
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	return window;
}

void center_window(GLFWwindow* window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if (monitor == NULL) {
		std::cout << "Could not find a monitor" << std::endl;
		return;
	}
	int mWidth, mHeight, mX, mY;
	glfwGetMonitorWorkarea(monitor, &mX, &mY, &mWidth, &mHeight);

	int x = mX + mWidth / 2 - width / 2, y = mY + mHeight / 2 - height / 2;
	glfwSetWindowPos(window, x, y);
}



void initialize_glfw() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
}

void initialize_glad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
}

void set_callbacks(GLFWwindow* window) {
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keypress_callback);
}



void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void keypress_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		std::cout << "exit" << '\n';
	}
	std::cout << key << '\n';
}