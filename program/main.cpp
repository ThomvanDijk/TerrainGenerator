/**
 * Copyright 2018 Thom van Dijk <https://github.com/ThomvanDijk>
 * Initial commit
 *
 * Contains initial code from http://www.opengl-tutorial.org
 */

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.h"


#define SWIDTH 800
#define SHEIGHT 600

#define VERTEXSHADER "shaders/sprite.vert"		// vertexshader name
#define FRAGMENTSHADER "shaders/sprite.frag"	// fragmentshader name

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(SWIDTH, SHEIGHT, "Hello Cube", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Background color
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders(VERTEXSHADER, FRAGMENTSHADER);

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 90� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 4, 0), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// GRID COORDINATES
	float points[] =
	{
		-0.5f,  0.5f,  0.0f, // x, y, z
		 0.5f,  0.5f,  1.0f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.0f
	};

	int indices[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	// VERTEX BUFFER
	GLuint vbo = 0; // Vertex Buffer Object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeof(points), points, GL_STATIC_DRAW);

	GLuint eab = 0; // Element Array Buffer
	glGenBuffers(1, &eab);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint vao = 0; // Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		// Draw the GL_TRIANGLE_STRIP!
		glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);

		// Poll events for mouse and key input
		glfwPollEvents();
	}

	// Cleanup vbo, eab, vao and shader
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &eab);
	glDeleteBuffers(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
